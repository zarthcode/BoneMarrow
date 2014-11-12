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
		WLAN_USART_TxComplete();
	}
}

/*
HAL_USART_TxHalfCpltCallback(USART_HandleTypeDef *husart);
{

}
*/

void HAL_USART_TxRxCpltCallback(USART_HandleTypeDef *husart)
{

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