#include "CC3000_Impl.h"
#include "stm32f4xx_hal.h"
#include "Semihosting.h"


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

	switch (GPIO_Pin)
	{
	case GPIO_PIN_4: // EXTI4
		// CC3000 Interrupt
		if (wlan_irq_enabled)
		{
			// Call interrupt/receive routine
			WLAN_nIRQ_Event();
		}


		break;
	default:
		printf_semi("HAL_GPIO_EXTI_Callback(%d) - Unhandled/Unimplemented EXTI.\n", GPIO_Pin);
		break;
	}


}