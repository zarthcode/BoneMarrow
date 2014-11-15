/// @file TI CC3000 USART3/SPI Implementation

#include <stm32f4xx_hal.h>
#include "cc3000_common.h"
#include "usart.h"
#include <stdbool.h>
#include "Semihosting.h"
#include "Delay.h"
#include <string.h>
#include <inttypes.h>


#define READ_OPCODE 0x03
#define WRITE_OPCODE 0x01
#define BUSY_OPCODE 0x00

#define HI(x) (x >> 8) & 0xFF
#define LO(x) x & 0xFF

typedef struct 
{

	GPIO_TypeDef* port;	// Port
	uint16_t pin;		// Pin

} Pin_Mapping;


Pin_Mapping PM_WLAN_CS = { .port = GPIOD, .pin = GPIO_PIN_3 };	// Isn't C99 grand?
Pin_Mapping PM_WLAN_EN = { .port = GPIOD, .pin = GPIO_PIN_5 };
Pin_Mapping PM_WLAN_IRQ = { .port = GPIOD, .pin = GPIO_PIN_4 };	// EXTI4 is connected (EVP4)



CC3000_SPIRecvHandlerType wlan_CC3000_RxHandler = NULL;


uint8_t wlan_tx_buffer[CC3000_MAXIMAL_TX_SIZE];			/// WLAN Transmit buffer
uint8_t wlan_rx_buffer[CC3000_MAXIMAL_RX_SIZE];			/// WLAN Receive buffer

// const uint8_t wlan_rx_header_seq[] = { 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF };		// Raw Values
const uint8_t wlan_rx_header_seq[] = { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF };		// Converted to USART LSB-first format

uint16_t wlan_tx_size = 0;								/// Amount of data in the tx buffer for transmission

bool wlan_first_time_write = false;		/// WLAN 1st time write timing flag

volatile bool wlan_irq_enabled = false;		/// WLAN Interrupt Handling Flag
volatile bool wlan_irq_flag = false;			/// WLAN IRQ service flag

typedef enum
{
	
	WLAN_SPI_OFF,			///< WLAN SPI is deactivated.
	WLAN_SPI_INIT,			///< WLAN SPI has not had first-write or initialization.
	WLAN_SPI_IDLE,		 	///< WLAN SPI has been initialized - waiting for interrupt or send
	WLAN_SPI_TX,			///< WLAN SPI Tx operation is in progress.
	WLAN_SPI_RX_HEADER,		///< WLAN SPI Rx operation is in progress (reading first 10 bytes).
	WLAN_SPI_RX_DATA,		///< WLAN SPI Rx operation is in progress (reading additional data).
	WLAN_SPI_RX_PROCESSING,	///< WLAN SPI Rx operation is in progress (processing data).

} WLAN_SPIStateType;

volatile WLAN_SPIStateType wlan_spi_state = WLAN_SPI_OFF;

typedef enum
{
	WLAN_SPI_TX_IDLE,
	WLAN_SPI_TX_CS_ASSERTED,
	WLAN_SPI_TX_DATA_SENT
	
} WLAN_SPI_TXStateType;

WLAN_SPI_TXStateType wlan_spi_tx_state = WLAN_SPI_TX_IDLE;

/// TRUE when the rx buffer has data that needs to be processed.
volatile bool wlan_rx_pending;

/// Utility function to reverse buffer data to/from LSB first that the USART expects.
void USART_RBITBuffer(uint8_t* pBuffer, uint16_t bufferSize);

/// SPI Open Function.
void SpiOpen(CC3000_SPIRecvHandlerType pfRxHandler)
{

	// Ensure the device is being held in reset
	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_RESET);

	// Save the receive interrupt handler address.
	wlan_CC3000_RxHandler = pfRxHandler;

	// Initialize the /CS pin
	HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

	// Start the USART
	MX_USART3_Init();

	// Setup Ethernet interrupt priority
	/** @note I've stolen the Ethernet IRQ to represent a WiFi interrupt.
	  * It may be more proper to implement a true SVC handler to run code without
	  * starving other interrupt sources.
	  */
	HAL_NVIC_SetPriority(ETH_IRQn, 12, 1);
	HAL_NVIC_EnableIRQ(ETH_IRQn);

	// Setup USART3 interrupt for TC handling.
	HAL_NVIC_SetPriority(USART3_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	wlan_spi_state = WLAN_SPI_INIT;

}

void SpiClose()
{
	// Stop any ongoing transfers.
	// HAL_USART_DMAStop(&husart3);
	// IRQ Stop?

	// Disable external interrupt.
	WLAN_IRQDisable();

	// Disable service interrupt
	HAL_NVIC_DisableIRQ(ETH_IRQn);

	// Disable USART3 interrupt
	HAL_NVIC_DisableIRQ(USART3_IRQn);

	// Power down the USART
	HAL_USART_DeInit(&husart3);

	// Hold WLAN in reset.
	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_RESET);

	wlan_spi_state = WLAN_SPI_OFF;

	wlan_spi_tx_state = WLAN_SPI_TX_IDLE;

}

long SpiWrite(uint8_t* pUserBuffer, unsigned short usLength)
{

	// Ensure that the device is ready
	if (husart3.State != HAL_USART_STATE_READY)
	{
		printf_semi("SpiWrite() - USART not ready.");
#ifdef DEBUG
		__BKPT(0);
#endif
	}

	
	// The entire transaction must be 16-bit aligned - meaning an optional padding byte.
	bool bNeedsPadding = ((usLength + 5) % 2 == 0) ? false : true;

	// Prepare the tx buffer.
	// Write the header
	pUserBuffer[0] = WRITE_OPCODE;
	pUserBuffer[1] = HI(usLength + (bNeedsPadding ? 1 : 0));	// Might be better to simply write to *((uint_16*)(&tx_Header[1])...
	pUserBuffer[2] = LO(usLength + (bNeedsPadding ? 1 : 0));	// But then we might need to configure endianness someday...
	pUserBuffer[3] = BUSY_OPCODE;
	pUserBuffer[4] = BUSY_OPCODE;

	wlan_tx_size = usLength + 5 /* Header size */ + (bNeedsPadding ? 1 : 0);
	
	USART_RBITBuffer(pUserBuffer, wlan_tx_size);

	if (bNeedsPadding)
	{
		pUserBuffer[5 + usLength] = 0x00;
	}

	// First time write?
	if (false == wlan_first_time_write)
	{
		// The first write attempt is a bit different.
		// We're going to do it all right here to ensure correct timing.
		// Disable incoming IRQ from WLAN
		WLAN_IRQDisable();


		// Wait for IRQ low
		while (GPIO_PIN_RESET != HAL_GPIO_ReadPin(PM_WLAN_IRQ.port, PM_WLAN_IRQ.pin))
		{
			// Wait
		}

		// Wait 50uS
		Delay_uS(55);

		// Assert CS
		HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_RESET);


		// Write first 4 bytes
		if (HAL_OK != HAL_USART_Transmit(&husart3, pUserBuffer, 4, 1000))
		{
			printf_semi("HAL_USART_Transmit_*() (first write) attempt failed.");
#ifdef DEBUG
			__BKPT(0);
#endif
			return 1;
		}

		// Delay at least 50uS
		Delay_uS(55);

		// Transmit the rest of the packet.
		if (HAL_OK != HAL_USART_Transmit(&husart3, pUserBuffer + 4, usLength + 1 + (bNeedsPadding ? 1 : 0), 1000))
		{
			printf_semi("HAL_USART_Transmit_*() attempt failed.");
#ifdef DEBUG
			__BKPT(0);
#endif
			return 1;
		}

		// Re-enable IRQ processing
		WLAN_IRQEnable();

		// Dassert nCS
		HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

		
		// Operation completed.
		wlan_first_time_write = true;

		wlan_spi_state = WLAN_SPI_IDLE;


	}
	else // Generic Host Write
	{
		// Set the state
		wlan_spi_state = WLAN_SPI_TX;

		// Assert nCS
		wlan_spi_tx_state = WLAN_SPI_TX_CS_ASSERTED;
		HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_RESET);

		// Wait for nIRQ.


	}



	return 0;
}

void WLAN_IRQEnable()
{

	wlan_irq_enabled = true;
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

void WLAN_IRQDisable()
{

	// Disable interrupt
	wlan_irq_enabled = false;
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);
}

void WLAN_WriteENPin(unsigned char val)
{
	/// @todo sWriteWlanPin() - Assert IS_GPIO_PIN_ACTION(val)

	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, val ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// Reset the 1st time write flag
	if (0 == val)
	{
		wlan_first_time_write = false;

	}
}

long WLAN_ReadIRQPin(void)
{

	return HAL_GPIO_ReadPin(PM_WLAN_IRQ.port, PM_WLAN_IRQ.pin) ? 1 : 0;

}

void SpiResumeSpi()
{

	// The SpiResumeSpi function is called after the received packet is 
	// processed by the CC3000 host driver code in the context of the receive handler.
	wlan_spi_state = WLAN_SPI_IDLE;

	WLAN_IRQEnable();
}

void USART_RBITBuffer(uint8_t* pBuffer, uint16_t bufferSize)
{

			for (uint16_t i = 0; i < bufferSize; i++)
			{
				uint32_t temp = __RBIT(pBuffer[i]);
				pBuffer[i] = temp >> 24;
			}
}

void WLAN_Service(void)
{
	switch (wlan_spi_state)
	{
	case WLAN_SPI_RX_PROCESSING:
		{
			// There is a buffer of data that's ready to be processed.

			// We need to convert the data to MSB-first.
			uint16_t DataSize = (wlan_rx_buffer[3] << 8) | (wlan_rx_buffer[4]);	// SMSB,SLSB
			USART_RBITBuffer(wlan_rx_buffer + 5, DataSize);		// Skip the header, it's already parsed.


			// Start processing
			wlan_CC3000_RxHandler(wlan_rx_buffer + 5);

			/// @bug Determine if WLAN_Service() needs to check for additional processing, after completion - without exiting and re-entering this interrupt.

			// IMPORTANT: Don't go back to the idle state here.  The CC3000 driver likely has, already.  Especially in the case that it is/has sent a return packet.

		}
		break;
	default:
		// No service needed.
		printf_semi("WLAN_Service() entered without need for processing.\n");
		break;
	}

}

void SpiRead()
{
	switch (wlan_spi_state)
	{
	case WLAN_SPI_IDLE:
		{
			// IRQ was received from CC3000.

			// Assure that no write operation is in progress...

			wlan_spi_state = WLAN_SPI_RX_HEADER;

			// Assert nCS
			HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_RESET);

			// Disable IRQ
			WLAN_IRQDisable();

			// Read 10 bytes: 5 bytes of data, 5 (minimum) bytes of data.
			HAL_StatusTypeDef res = HAL_USART_TransmitReceive_DMA(&husart3, (uint8_t*)wlan_rx_header_seq, wlan_rx_buffer, 10);

			if (res != HAL_OK)
			{
				printf_semi("SpiRead - HAL_USART_Receive_DMA() (WLAN_SPI_IDLE) failed.\n");
			}

			// We'll be back!
		}
		return;


	case WLAN_SPI_RX_HEADER:
		{


			// Convert the header (to account for USART sending LSB first.)
			USART_RBITBuffer(wlan_rx_buffer, 5);

			// Parse the header to determine how much data is waiting
			uint16_t DataSize = (wlan_rx_buffer[3] << 8) | (wlan_rx_buffer[4]);	// SMSB,SLSB

			// If there is more data waiting, read it as a block.
			if (DataSize > 5)
			{
				// Setup IT/DMA operation to read remaining data.
				wlan_spi_state = WLAN_SPI_RX_DATA;
				HAL_StatusTypeDef res = HAL_USART_Receive_DMA(&husart3, wlan_rx_buffer + 10, DataSize - 5);

				if (res != HAL_OK)
				{
					printf_semi("SpiRead - HAL_USART_Receive_DMA() (WLAN_SPI_RX_HEADER) failed.\n");
				}

			}
			else
			{
				// Deassert nCS
				HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

				// Pass the data to the receive handler.
				wlan_spi_state = WLAN_SPI_RX_PROCESSING;
				HAL_NVIC_SetPendingIRQ(ETH_IRQn);

			}



		}
		return;
	case WLAN_SPI_RX_DATA:
		{

			WLAN_IRQDisable();
			
			// Deassert nCS
			HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);


			// Data has been received.  Process it.
			wlan_spi_state = WLAN_SPI_RX_PROCESSING;
			HAL_NVIC_SetPendingIRQ(ETH_IRQn);
		}
		return;

	default:
		// Looks like we are in a bad/unhandled state.
		printf_semi("SpiRead() - unhandled wlan_spi_state(%d)\n", wlan_spi_state);
#ifdef DEBUG
		__BKPT(0);
#endif
	}
		
	
}

void WLAN_nIRQ_Event(void)
{

	if (!wlan_irq_enabled)
	{
		printf_semi("WLAN_IRQ_Event() called while IRQ disabled!\n");
#ifdef DEBUG
		__BKPT(0);
#endif
		return;
	}

	/// If IRQ is active, then perform a receive.
	switch (wlan_spi_state)
	{
	case WLAN_SPI_OFF:
		// IRQ should not happen in this state.  Unless device has been enabled.
		printf_semi("Error: WLAN IRQ Received while in WLAN_SPI_OFF state.(IRQ not handled)\n");
		break;
		/* WLAN_SPI_INIT is handled by the cc3000 driver, deprecate.
	case WLAN_SPI_INIT:
		// IRQ has been received as a result of power-up.
		// Wait 7ms before asserting CS


		break;
		*/
	case WLAN_SPI_IDLE:
		// Device wants to initiate a rx operation.
		SpiRead();

		break;
	case WLAN_SPI_TX:
		switch (wlan_spi_tx_state)
		{
		case WLAN_SPI_TX_CS_ASSERTED:
			{
				// nCS was asserted, IRQ has been asserted in response - time to start sending.

				// Write the header
				// Write the payload
				// Write the padding byte, if needed.
				wlan_spi_tx_state = WLAN_SPI_TX_DATA_SENT;
				HAL_StatusTypeDef res = HAL_USART_Transmit_DMA(&husart3, wlan_tx_buffer, wlan_tx_size);
				if (HAL_OK != res)
				{
					printf_semi("WLAN_nIRQ_Event() - HAL_USART_Transmit_DMA() attempt failed (%d).\n", res);
#ifdef DEBUG
					__BKPT(0);
#endif
				}
			}
			break;
		
		default:
			printf_semi("WLAN_nIRQ_Event() - WLAN_SPI_TX state (%d) invalid.\n", wlan_spi_tx_state);
#ifdef DEBUG
			__BKPT(0);
#endif
			break;
		}
		break;
	case WLAN_SPI_RX_HEADER:
		// If a RX is already in-progress, this isn't a valid event. 
		printf_semi("WLAN_nIRQ_Event() - Error - Current state is WLAN_SPI_RX_HEADER (%d).\n", wlan_spi_state);
#ifdef DEBUG
		__BKPT(0);
#endif
		break;
	case WLAN_SPI_RX_DATA:
		// If a RX is already in-progress, this isn't a valid event. 
		printf_semi("WLAN_nIRQ_Event() - Error - Current state is WLAN_SPI_RX_DATA (%d).\n", wlan_spi_state);
#ifdef DEBUG
		__BKPT(0);
#endif
		break;
	case WLAN_SPI_RX_PROCESSING:
		// If a RX is already in-progress, this isn't a valid event. 
		// (It's likely that we're processing the received data too slowly?)
		printf_semi("WLAN_nIRQ_Event() - Error - Current state is WLAN_SPI_RX_PROCESSING (%d).\n", wlan_spi_state);
#ifdef DEBUG
		__BKPT(0);
#endif
		break;
	}

	return;

}

/// This handler has been repurposed to serve as a place to process received wifi packets
void ETH_IRQHandler(void)
{
	// Clear IRQ
	HAL_NVIC_ClearPendingIRQ(ETH_IRQn);

	WLAN_Service();

// SpiResumeSpi() may take care of this...
//	WLAN_IRQEnable();

}

void WLAN_USART_TxComplete(void)
{


	// Operation Complete
	wlan_spi_tx_state = WLAN_SPI_TX_IDLE;
	wlan_spi_state = WLAN_SPI_IDLE;

	// Deassert nCS
	// HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);
	HAL_GPIO_TogglePin(PM_WLAN_CS.port, PM_WLAN_CS.pin);

}
