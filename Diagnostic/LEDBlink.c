#include "stm32f4xx_hal_conf.h"
#include <stm32f4xx_hal.h>

#ifdef __cplusplus
extern "C"
#endif
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

int main(void)
{
	// LED connections
	// TIM3_1 - D1R
	// TIM3_2 - D1G	
	// TIM3_3 - D1B
	// TIM3_4 - D6 (BLUE LED)

	// TIM8_1 - D2R
	// TIM8_2 - D2G
	// TIM8_3 - D2B

	// TIM4_1 - D3R
	// TIM4_2 - D3G
	// TIM4_3 - D3B

	// TIM1_1 - D4R
	// TIM1_2 - D4G
	// TIM1_3 - D4B
	// TIM1_4 - D8 (RED LED)

	// TIM5_1 - D5R
	// TIM5_2 - D5G
	// TIM5_3 - D5B

	HAL_Init();

	__GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_6;

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_LOW;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	for (;;)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(250);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_Delay(250);
	}
}
