/// @file TI CC3000 USART3/SPI Implementation

#include <stm32f4xx_hal.h>
#include "cc3000_common.h"
#include "usart.h"
#include <stdbool.h>
#include "Semihosting.h"
#include "Delay.h"
#include <string.h>


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

}

void SpiClose()
{
	// Stop any ongoing transfers.
	// HAL_USART_DMAStop(&husart3);
	// IRQ Stop?

	// Disable external interrupt.
	sWlanInterruptDisable();

	// Disable service interrupt
	HAL_NVIC_DisableIRQ(ETH_IRQn);

	// Power down the USART
	HAL_USART_DeInit(&husart3);


	// Hold WLAN in reset.
	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_RESET);

}

long SpiWrite(uint8_t* pUserBuffer, unsigned short usLength)
{

	// Ensure that the device is ready
	if (husart3.State != HAL_USART_STATE_READY)
	{
		printf_semi("USART not ready.");
#ifdef DEBUG
		__BKPT(0);
#endif
	}

	// Disable incoming IRQ from WLAN
	sWlanInterruptDisable();
	
	// The entire transaction must be 16-bit aligned - meaning an optional padding byte.
	bool bNeedsPadding = ((usLength + 5) % 2 == 0) ? false : true;

	// Prepare the tx buffer.
	// Write the header
	pUserBuffer[0] = WRITE_OPCODE;
	pUserBuffer[1] = HI(usLength + (bNeedsPadding ? 1 : 0));	// Might be better to simply write to *((uint_16*)(&tx_Header[1])...
	pUserBuffer[2] = LO(usLength + (bNeedsPadding ? 1 : 0));	// But then we might need to configure endianness someday...
	pUserBuffer[3] = BUSY_OPCODE;
	pUserBuffer[4] = BUSY_OPCODE;

	if (bNeedsPadding)
	{
		pUserBuffer[6 + usLength] = 0x00;
	}

	// First time write?
	if (false == wlan_first_time_write)
	{
		// The first write attempt is a bit different.
		// We're going to do it all right here to ensure correct timing.


		// Wait for IRQ low
		while (!HAL_GPIO_ReadPin(PM_WLAN_IRQ.port, PM_WLAN_IRQ.pin))
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

		// Operation completed.
		wlan_first_time_write = true;

	}
	else // Generic Host Write
	{

		// Assert nCS
		HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_RESET);

		// Wait for IRQ low
		while (!HAL_GPIO_ReadPin(PM_WLAN_IRQ.port, PM_WLAN_IRQ.pin))
		{
			// Wait
		}

		// Delay
		Delay_uS(10);

		// Write the header
		// Write the payload
		// Write the padding byte, if needed.
		if (HAL_OK != HAL_USART_Transmit(&husart3, pUserBuffer, usLength + 5 + (bNeedsPadding ? 1 : 0), 1000))
		{
			printf_semi("HAL_USART_Transmit_*() attempt failed.");
#ifdef DEBUG
			__BKPT(0);
#endif
			return 1;
		}

		// Delay
		Delay_uS(10);

		// Deassert nCS
		


	}

	// Deassert nCS
	HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

	// Re-enable /IRQ
	sWlanInterruptEnable();


	return 0;
}

void sWlanInterruptEnable()
{

	wlan_irq_enabled = true;
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

void sWlanInterruptDisable()
{

	// Disable interrupt
	wlan_irq_enabled = false;
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);
}

void sWriteWlanPin(unsigned char val)
{
	/// @todo sWriteWlanPin() - Assert IS_GPIO_PIN_ACTION(val)

	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, val ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// Reset the 1st time write flag
	if (0 == val)
	{
		wlan_first_time_write = false;

	}
}

long sReadWlanInterruptPin(void)
{

	return HAL_GPIO_ReadPin(PM_WLAN_IRQ.port, PM_WLAN_IRQ.pin) ? 1 : 0;

}

void SpiResumeSpi()
{

	// The SpiResumeSpi function is called after the received packet is 
	// processed by the CC3000 host driver code in the context of the receive handler.
	wlan_spi_state = WLAN_SPI_IDLE;

	sWlanInterruptEnable();
}

void WLAN_Service(void)
{
	switch (wlan_spi_state)
	{
	case WLAN_SPI_RX_PROCESSING:
		// There is a buffer of data that's ready to be processed.
		wlan_CC3000_RxHandler(wlan_rx_buffer);

		/// @bug Determine if WLAN_Service() needs to check for additional processing, after completion.

		break;
	default:
		// No service needed.
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
			sWlanInterruptDisable();

			// Read 10 bytes: 5 bytes of data, 5 (minimum) bytes of data.
			HAL_StatusTypeDef res = HAL_USART_Receive_DMA(&husart3, wlan_rx_buffer, 10);

			if (res != HAL_OK)
			{
				printf_semi("SpiRead - HAL_USART_Receive_DMA() (WLAN_SPI_IDLE) failed.\n");
			}

			// We'll be back!
		}
		return;


	case WLAN_SPI_RX_HEADER:
		{

			// Parse the header to determine how much data is waiting
			uint16_t* pDataSize = &wlan_rx_buffer[1];	// SMSB,SLSB

			// If there is more data waiting, read it as a block.
			if (*pDataSize > 5)
			{
				// Setup IT/DMA operation to read remaining data.
				wlan_spi_state = WLAN_SPI_RX_DATA;
				HAL_StatusTypeDef res = HAL_USART_Receive_DMA(&husart3, wlan_rx_buffer, *pDataSize - 5);

				if (res != HAL_OK)
				{
					printf_semi("SpiRead - HAL_USART_Receive_DMA() (WLAN_SPI_RX_HEADER) failed.\n");
				}

			}
			else
			{
				// Pass the data to the receive handler.
				wlan_spi_state = WLAN_SPI_RX_PROCESSING;

				/// @todo CC3000 SpiRead() - Trigger low-priority software interrupt to process received data.
				

			}



		}
		return;
	case WLAN_SPI_RX_DATA:
		{

			// Data has been received.  Process it.
			wlan_spi_state = WLAN_SPI_RX_PROCESSING;
		}
		return;

	default:
		// Looks like we are in a bad/unhandled state.
		printf_semi("SpiRead() - unhandled wlan_spi_state(%d)\n", wlan_spi_state);
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
		printf_semi("Error: WLAN IRQ Received while in WLAN_SPI_OFF state.\n");
		break;

	case WLAN_SPI_INIT:
		// IRQ has been received as a result of power-up.
		// Wait 7ms before asserting CS

		break;
	case WLAN_SPI_IDLE:
		// Device wants to initiate a rx operation.
		SpiRead();

		break;
	case WLAN_SPI_TX:
		// nCS was asserted, IRQ has been raised in response - time to start sending.
		break;
	case WLAN_SPI_RX:
		// If a RX is already in-progress, this isn't a valid event. 
		// (It's likely that we're processing the received data?)
		break;
	}

	return;

}

/// This handler has been repurposed to serve as a place to process received wifi packets
void ETH_IRQHandler(void)
{
	// Clear IRQ
	HAL_NVIC_ClearPendingIRQ(ETH_IRQn);

	// Process the event
	if (wlan_spi_state == WLAN_SPI_RX_PROCESSING)
	{
		wlan_CC3000_RxHandler(wlan_rx_buffer);
	}

}
