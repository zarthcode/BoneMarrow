#include "Delay.h"
#include <stm32f4xx_hal.h>
#include "Semihosting.h"
#include <inttypes.h>
#include "tim.h"

// extern uint32_t SystemCoreClock;

void Delay_uS(uint16_t us)
{
	// TIM6 is only 16 bits, so we need to ensure there's no overflow.
	/// \todo Add overflow and 32-bit support to Delay_uS

	// Reset the count and start the timer
	htim6.Instance->CNT = 0;
	htim6.Init.Period = us;
	htim6.Init.RepetitionCounter = 
	HAL_TIM_Base_Init(&htim6);
	HAL_TIM_Base_Start(&htim6);
	
	/// \todo Delay_uS - Use the timer to generate an event, allow processor to __WFE() sleep.
	while (htim6.Instance->CR1 & TIM_CR1_CEN);

	HAL_TIM_Base_Stop(&htim6);

	
}

void Delay_Init(void)
{
	/*
	if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
	{
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
	*/

	// Start TIM6 (Configured by STM32CubeMX for 1uS pulses)
	// Not starting the timer here, for now, it's only 16bit, and is reset manually by calling Delay_uS.
	// If needed for benchmarking/timing, then TIM6 and TIM7 need to be cascaded into a 32-bit timer.

}

uint32_t DWT_Get(void)
{
//	return DWT->CYCCNT;
}


