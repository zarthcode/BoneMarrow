/// @file TI CC3000 USART3/SPI Implementation

#include <stm32f4xx_hal.h>
#include "cc3000_common.h"
#include "usart.h"
#include <stdbool.h>
#include "Semihosting.h"
#include "Delay.h"


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
Pin_Mapping PM_WLAN_INT = { .port = GPIOD, .pin = GPIO_PIN_4 };


CC3000_SPIRecvHandlerType _CC3000_SPIHandler = NULL;


uint8_t wlan_tx_buffer[CC3000_MAXIMAL_TX_SIZE];			/// WLAN Transmit buffer
uint8_t wlan_rx_buffer[CC3000_MAXIMAL_RX_SIZE];			/// WLAN Receive buffer

bool wlan_first_time_write = false;		/// WLAN 1st time write timing flag

bool wlan_int_enabled = false;		/// WLAN Interrupt Handling Flag


/// SPI Open Function.
void SpiOpen(CC3000_SPIRecvHandlerType pfRxHandler)
{

	// Ensure the device is being held in reset
	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_RESET);


	// Save the interrupt handler handle.
	_CC3000_SPIHandler = pfRxHandler;

	// Initialize the /CS pin
	HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

	// Start the USART
	MX_USART3_Init();

	/// @todo Enable EXTI handling for CC3x00

	/*Configure GPIO pin : PD4 */
	{
		GPIO_InitTypeDef GPIO_InitStruct;

		GPIO_InitStruct.Pin = PM_WLAN_INT.pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	}


	// Remove the device from reset
	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_SET);
	HAL_Delay(1);

	// Interrupt will occur, 


}

void SpiClose()
{
	// Stop any ongoing transfers.
	// HAL_USART_DMAStop(&husart3);
	// IRQ Stop?

	// Power down the USART
	HAL_USART_DeInit(&husart3);

	// Disable external interrupt.
	sWlanInterruptDisable();

	// Hold WLAN in reset.
	sWriteWlanPin(0);

}

long SpiWrite(uint8_t* pUserBuffer, unsigned short usLength)
{
	/// @todo  Implement Dynamic DMA channel allocation with a high priority

	/// @todo Acquire DMA channel for USART use

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

		// Deassert CS
		HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

		// Toggle Nshutdown
		{
			HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_RESET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, GPIO_PIN_SET);
		}

		// Wait for IRQ low
		while (!HAL_GPIO_ReadPin(PM_WLAN_INT.port, PM_WLAN_INT.pin))
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
			printf_semi("HAL_USART_Transmit_*() attempt failed.");
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
		while (!HAL_GPIO_ReadPin(PM_WLAN_INT.port, PM_WLAN_INT.pin))
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
		// Operation is complete once nIRQ deasserts


	}

	// Deassert nCS
	HAL_GPIO_WritePin(PM_WLAN_CS.port, PM_WLAN_CS.pin, GPIO_PIN_SET);

	// Re-enable /IRQ
	sWlanInterruptEnable();


	return 0;
}

void sWlanInterruptEnable()
{

	// True EXTI not yet implemented.
	wlan_int_enabled = true;


}

void sWlanInterruptDisable()
{

	// True EXTI not yet implemented.
	// Disable interrupt
	wlan_int_enabled = false;

}

void sWriteWlanPin(unsigned char val)
{

	HAL_GPIO_WritePin(PM_WLAN_EN.port, PM_WLAN_EN.pin, val ? GPIO_PIN_SET : GPIO_PIN_RESET);

	// Reset the 1st time write flag
	if (0 == val)
	{
		wlan_first_time_write = false;
	}
}

long sReadWlanInterruptPin(void)
{

	return HAL_GPIO_ReadPin(PM_WLAN_INT.port, PM_WLAN_INT.pin) ? 1 : 0;

}

void SpiResumeSpi()
{

}

void SpiRead()
{
	// IRQ was received from CC3000.

	/// @todo Make SpiRead() a part of the IRQ handler, state-based.

	// Assure that no write operation is in progress...
	// Assert nCS


	// Read 10 bytes: 5 bytes of data, 10 (minimum) bytes of data.
	
	// Parse the header to determine how much data is waiting

	// If there is more data waiting, read it as a block.

	/// @todo Setup IT/DMA operation to read remaining data.

}
