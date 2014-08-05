#include <stm32f4xx_hal.h>
#include "tim.h"
typedef enum LED { D1, D2, D3, D4, D5 } LEDType;

void SetColor(LEDType led, int R, int G, int B);



/*
This is just a skeleton, directly access the RGB lights and blink em.

TIM3_CH1 - PC6 - D1_RED
TIM3_CH2 - PC7 - D1_GREEN
TIM3_CH3 - PC8 - D1_BLUE
TIM3_CH4 - PC9 - Blue LED

TIM8_CH1 - PI5 - D2_RED
TIM8_CH2 - PI6 - D2_GREEN
TIM8_CH3 - PI7 - D2_BLUE

TIM4_CH1 - PD12 - D3_RED
TIM4_CH2 - PD13 - D3_GREEN
TIM4_CH3 - PD14 - D3_BLUE

TIM1_CH1 - PE9 - D4_RED
TIM1_CH2 - PE11 - D4_GREEN
TIM1_CH3 - PE13 - D4_BLUE
TIM1_CH4 - PE14 - Red LED

TIM5_CH1 - PH10 - D5_RED
TIM5_CH2 - PH11 - D5_BLUE
TIM5_CH3 - PH12 - D5_GREEN

*/


void initPWM(void);

int app_postinit(void)
{


	// activate the TIM1 & TIM8 outputs
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
	
	// Disable CCPC on advanced timers.
	htim1.Instance->CR2 &= ~(TIM_CR2_CCPC);
	htim8.Instance->CR2 &= ~(TIM_CR2_CCPC);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);
}

int app_main(void)
{
	/*
	// Check button state
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
	{
		// Enable the timers for 10s
		HAL_Delay(10000);

		// Disable the PWM units.
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);

		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);

		HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_3);

		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);

		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
		HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_3);
	}
	*/

}
/*
void HAL_SYSTICK_Callback(void)
{
	HAL_Delay(1);
}
*/
void blinkMain(void)
{
	HAL_Init();


	__GPIOC_CLK_ENABLE();
	__GPIOI_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();
	

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);


	GPIO_InitStructure.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);


	GPIO_InitStructure.Pin = GPIO_PIN_9 | GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOH, &GPIO_InitStructure);



	// Disable Bluetooth (PI1)

	GPIO_InitStructure.Pin = GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_1, GPIO_PIN_SET);


	// Disable Fast charge

	GPIO_InitStructure.Pin = GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_15, GPIO_PIN_SET);


	// Turn Red and Blue LEDs on.
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_RESET);

	int rgb = 0;
	for (int rgb = 0; rgb < 7; rgb++)
	{
		// TODO: Check battery charger status. Blink RED led if charging.

		switch (rgb)
		{
		case 0:
		{
			// Blink white quickly for 4S
			int i = 4;
			for (i = 4 * 10; i > 0; i--)
			{
				SetColor(D1, 1, 1, 1);
				SetColor(D2, 1, 1, 1);
				SetColor(D3, 1, 1, 1);
				SetColor(D4, 1, 1, 1);
				SetColor(D5, 1, 1, 1);
				HAL_Delay(50);

				SetColor(D1, 0, 0, 0);
				SetColor(D2, 0, 0, 0);
				SetColor(D3, 0, 0, 0);
				SetColor(D4, 0, 0, 0);
				SetColor(D5, 0, 0, 0);
				HAL_Delay(50);
			}
			break;
		}
		case 1:
			// Red
			SetColor(D1, 1, 0, 0);
			SetColor(D2, 1, 0, 0);
			SetColor(D3, 1, 0, 0);
			SetColor(D4, 1, 0, 0);
			SetColor(D5, 1, 0, 0);
			break;
		case 2:
			// Purple
			SetColor(D1, 1, 1, 0);
			SetColor(D2, 1, 1, 0);
			SetColor(D3, 1, 1, 0);
			SetColor(D4, 1, 1, 0);
			SetColor(D5, 1, 1, 0);
			break;
		case 3:
			// Blue
			SetColor(D1, 0, 1, 0);
			SetColor(D2, 0, 1, 0);
			SetColor(D3, 0, 1, 0);
			SetColor(D4, 0, 1, 0);
			SetColor(D5, 0, 1, 0);
			break;
		case 4:
			// Cyan
			SetColor(D1, 0, 1, 1);
			SetColor(D2, 0, 1, 1);
			SetColor(D3, 0, 1, 1);
			SetColor(D4, 0, 1, 1);
			SetColor(D5, 0, 1, 1);
			break;
		case 5:
			// Green
			SetColor(D1, 0, 0, 1);
			SetColor(D2, 0, 0, 1);
			SetColor(D3, 0, 0, 1);
			SetColor(D4, 0, 0, 1);
			SetColor(D5, 0, 0, 1);
			break;
		case 6:
			// Yellow
			SetColor(D1, 1, 0, 1);
			SetColor(D2, 1, 0, 1);
			SetColor(D3, 1, 0, 1);
			SetColor(D4, 1, 0, 1);
			SetColor(D5, 1, 0, 1);
			break;

		default:
			// rgb = 0;
			break;
		}


		HAL_Delay(1000);


	}

	SetColor(D1, 1, 0, 0);
	SetColor(D2, 1, 0, 0);
	SetColor(D3, 1, 0, 0);
	SetColor(D4, 1, 0, 0);
	SetColor(D5, 1, 0, 0);
	
	
}

void initPWM(void)
{

	
}

void SetColor(LEDType led, int Red, int Green, int Blue)
{
	switch (led)
	{
	case D1:

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, Red ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, Green ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, Blue ? GPIO_PIN_RESET : GPIO_PIN_SET);
		
		break;
	case D2:

		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_5, Red ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_6, Green ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOI, GPIO_PIN_7, Blue ? GPIO_PIN_RESET : GPIO_PIN_SET);

		break;	
	case D3:

		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, Red ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, Green ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, Blue ? GPIO_PIN_RESET : GPIO_PIN_SET);
		
		break;
	case D4:

		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, Red ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, Green ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, Blue ? GPIO_PIN_RESET : GPIO_PIN_SET);

		break;
	case D5:

		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_10, Red ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_11, Green ? GPIO_PIN_RESET : GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOH, GPIO_PIN_12, Blue ? GPIO_PIN_RESET : GPIO_PIN_SET);

		break;
	}
}
