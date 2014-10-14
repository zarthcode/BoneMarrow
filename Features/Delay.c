#include "Delay.h"
#include <stm32f4xx.h>

extern uint32_t SystemCoreClock;

void Delay_uS(uint32_t us)
{

	int32_t tp = DWT_Get() + us * (SystemCoreClock / 1000000);
	while (((int32_t)DWT_Get() - tp) < 0);
	
}

void Delay_Init(void)
{
	if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk))
	{
		CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
		DWT->CYCCNT = 0;
		DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
	}
}

uint32_t DWT_Get(void)
{
	return DWT->CYCCNT;
}


