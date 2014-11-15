/// @file Implementation of USART callback routines


#include "CC3000_Impl.h"
// #include "stm32f4xx_hal.h"
#include "Semihosting.h"

#include "usart.h"


void HAL_USART_TxCpltCallback(USART_HandleTypeDef *husart)
{
	if (&husart3 == husart)
	{
		// Wifi Handler

//			WLAN_USART_TxComplete();

		// DMA transfer is complete, but not necessarily the data transmission (the USART can buffer 1 bytes, plus a byte in the shift register!)
		// Enable USART3 interrupt
		__USART_ENABLE_IT(&husart3, USART_IT_TC);
	}
}

/*
HAL_USART_TxHalfCpltCallback(USART_HandleTypeDef *husart);
{

}
*/

void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart)
{

	if (&husart3 == husart)
	{
		// SPI Read Handler
		SpiRead();
	}
}

void HAL_USART_RxCpltCallback(USART_HandleTypeDef *husart)
{

	if (&husart3 == husart)
	{
		// SPI Read Handler
		SpiRead();
	}

}

/*
HAL_USART_RxHalfCpltCallback(USART_HandleTypeDef *husart);
{

}
*/

void HAL_USART_ErrorCallback(USART_HandleTypeDef *husart)
{

	printf_semi("HAL_USART_ErrorCallback() entered.\n");
#ifdef DEBUG
	__BKPT(0);
#endif // DEBUG


}

uint8_t strangenessCount = 0;

void USART3_IRQHandler(void)
{
	if (__HAL_USART_GET_FLAG(&husart3, USART_FLAG_TC) && __HAL_USART_GET_FLAG(&husart3, USART_FLAG_TXE))
	{
	//	__USART_DISABLE_IT(&husart3, USART_IT_TC);
		HAL_NVIC_ClearPendingIRQ(USART3_IRQn);
	//	HAL_NVIC_DisableIRQ(USART3_IRQn);
		__HAL_USART_CLEAR_FLAG(&husart3, USART_IT_TC);

		// Transmission complete.
		strangenessCount++;
		if (strangenessCount == 2)
		{
			WLAN_USART_TxComplete();
			strangenessCount = 0;
			__USART_DISABLE_IT(&husart3, USART_IT_TC);
		}
	}
	

}
