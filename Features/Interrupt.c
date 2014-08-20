#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "Button.h"
#include "LED.h"

#include <stdio.h>

/**
* @brief This function handles the 1ms systick
*/
void HAL_SYSTICK_Callback(void)
{
	// Update animation state
	LED_SystickHandler();
	ButtonSystickHandler();

}


void NMI_Handler(void)
{

	#ifdef DEBUG
	printf("NMI_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}


void HardFault_Handler(void)
{

	#ifdef DEBUG
	printf("HardFault_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}


void MemManage_Handler(void)
{

	#ifdef DEBUG
	printf("MemManage_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}


void BusFault_Handler(void)
{

	#ifdef DEBUG
	printf("BusFault_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}


void UsageFault_Handler(void)
{

	#ifdef DEBUG
	printf("UsageFault_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

/**/
void SVC_Handler(void)
{

	/*
	#ifdef DEBUG
//	printf("SVC_Handler called. (Unimplemented - HALT)\n");
__BKPT(0);
	#endif
	while (1) {};
	*/

}

void DebugMon_Handler(void)
{

	#ifdef DEBUG
	printf("DebugMon_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void PendSV_Handler(void)
{

	#ifdef DEBUG
	printf("PendSV_Handler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void WWDG_IRQHandler(void)
{

	#ifdef DEBUG
	printf("WWDG_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void PVD_IRQHandler(void)
{

	#ifdef DEBUG
	printf("PVD_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TAMP_STAMP_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TAMP_STAMP_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void RTC_WKUP_IRQHandler(void)
{

	#ifdef DEBUG
	printf("RTC_WKUP_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void FLASH_IRQHandler(void)
{

	#ifdef DEBUG
	printf("FLASH_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void RCC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("RCC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void EXTI0_IRQHandler(void)
{


	#ifdef DEBUG
	printf("EXTI0_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};


}

void EXTI1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("EXTI1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void EXTI2_IRQHandler(void)
{

	#ifdef DEBUG
	printf("EXTI2_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void EXTI3_IRQHandler(void)
{

	#ifdef DEBUG
	printf("EXTI3_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void EXTI4_IRQHandler(void)
{

	#ifdef DEBUG
	printf("EXTI4_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream0_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream0_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream2_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream2_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream3_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream3_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream4_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream4_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream5_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream5_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream6_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream6_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void ADC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("ADC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN1_TX_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN1_TX_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN1_RX0_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN1_RX0_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN1_RX1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN1_RX1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN1_SCE_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN1_SCE_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void EXTI9_5_IRQHandler(void)
{

	#ifdef DEBUG
	printf("EXTI9_5_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM1_BRK_TIM9_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM1_BRK_TIM9_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM1_UP_TIM10_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM1_UP_TIM10_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM1_TRG_COM_TIM11_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM1_CC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM1_CC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM2_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM2_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM3_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM3_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM4_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM4_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void I2C1_EV_IRQHandler(void)
{

	#ifdef DEBUG
	printf("I2C1_EV_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void I2C1_ER_IRQHandler(void)
{

	#ifdef DEBUG
	printf("I2C1_ER_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void I2C2_EV_IRQHandler(void)
{

	#ifdef DEBUG
	printf("I2C2_EV_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void I2C2_ER_IRQHandler(void)
{

	#ifdef DEBUG
	printf("I2C2_ER_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SPI1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SPI1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SPI2_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SPI2_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void USART1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("USART1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void USART2_IRQHandler(void)
{

	#ifdef DEBUG
	printf("USART2_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void USART3_IRQHandler(void)
{

	#ifdef DEBUG
	printf("USART3_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void EXTI15_10_IRQHandler(void)
{

	#ifdef DEBUG
	printf("EXTI15_10_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void RTC_Alarm_IRQHandler(void)
{

	#ifdef DEBUG
	printf("RTC_Alarm_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void OTG_FS_WKUP_IRQHandler(void)
{

	#ifdef DEBUG
	printf("OTG_FS_WKUP_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM8_BRK_TIM12_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM8_BRK_TIM12_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM8_UP_TIM13_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM8_UP_TIM13_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM8_TRG_COM_TIM14_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM8_CC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM8_CC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA1_Stream7_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA1_Stream7_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void FSMC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("FSMC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SDIO_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SDIO_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM5_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM5_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SPI3_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SPI3_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void UART4_IRQHandler(void)
{

	#ifdef DEBUG
	printf("UART4_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void UART5_IRQHandler(void)
{

	#ifdef DEBUG
	printf("UART5_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM6_DAC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM6_DAC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void TIM7_IRQHandler(void)
{

	#ifdef DEBUG
	printf("TIM7_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream0_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream0_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream2_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream2_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream3_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream3_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream4_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream4_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void ETH_IRQHandler(void)
{

	#ifdef DEBUG
	printf("ETH_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void ETH_WKUP_IRQHandler(void)
{

	#ifdef DEBUG
	printf("ETH_WKUP_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN2_TX_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN2_TX_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN2_RX0_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN2_RX0_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN2_RX1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN2_RX1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CAN2_SCE_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CAN2_SCE_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void OTG_FS_IRQHandler(void)
{

	#ifdef DEBUG
	printf("OTG_FS_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream5_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream5_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream6_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream6_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2_Stream7_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2_Stream7_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void USART6_IRQHandler(void)
{

	#ifdef DEBUG
	printf("USART6_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void I2C3_EV_IRQHandler(void)
{

	#ifdef DEBUG
	printf("I2C3_EV_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void I2C3_ER_IRQHandler(void)
{

	#ifdef DEBUG
	printf("I2C3_ER_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void OTG_HS_EP1_OUT_IRQHandler(void)
{

	#ifdef DEBUG
	printf("OTG_HS_EP1_OUT_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void OTG_HS_EP1_IN_IRQHandler(void)
{

	#ifdef DEBUG
	printf("OTG_HS_EP1_IN_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void OTG_HS_WKUP_IRQHandler(void)
{

	#ifdef DEBUG
	printf("OTG_HS_WKUP_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void OTG_HS_IRQHandler(void)
{

	#ifdef DEBUG
	printf("OTG_HS_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DCMI_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DCMI_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void CRYP_IRQHandler(void)
{

	#ifdef DEBUG
	printf("CRYP_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void HASH_RNG_IRQHandler(void)
{

	#ifdef DEBUG
	printf("HASH_RNG_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void FPU_IRQHandler(void)
{

	#ifdef DEBUG
	printf("FPU_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void UART7_IRQHandler(void)
{

	#ifdef DEBUG
	printf("UART7_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void UART8_IRQHandler(void)
{

	#ifdef DEBUG
	printf("UART8_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SPI4_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SPI4_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SPI5_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SPI5_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SPI6_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SPI6_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void SAI1_IRQHandler(void)
{

	#ifdef DEBUG
	printf("SAI1_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void LTDC_IRQHandler(void)
{

	#ifdef DEBUG
	printf("LTDC_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void LTDC_ER_IRQHandler(void)
{

	#ifdef DEBUG
	printf("LTDC_ER_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}

void DMA2D_IRQHandler(void)
{

	#ifdef DEBUG
	printf("DMA2D_IRQHandler called. (Unimplemented - HALT)\n");
	__BKPT(0);
	#endif
	while (1) {};

}