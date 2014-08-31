#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>		// C99 req'd.
#include "Semihosting.h"
#include "Button.h"
#include "tim.h"
#include "LED.h"
#include "IMU.h"
#include "Battery.h"

extern void initialise_monitor_handles(void);
bool Diag_ButtonTest(void);
bool Diag_ClockTest(void);
bool Diag_BatteryMeterTest(void);

int app_preinit(void)
{

	
}

int app_postinit(void)
{
#ifdef DEBUG
	HAL_EnableDBGSleepMode();
	HAL_EnableDBGStandbyMode();
	HAL_EnableDBGStopMode();

	__BKPT(0);
	// Enable debug module during all modes

#ifdef SEMIHOSTING
	// NOTE: SIGTRAP?  Run "mon arm semihosting enable" in your GDB session. Then continue.
	initialise_monitor_handles();

#endif
#endif

	// Greeting banner
	printf_semi("BoneMarrow™ Diagnostic Firmware\n©2014 ZarthCode LLC\n");

#ifdef __FPU_PRESENT
	printf_semi("__FPU_PRESENT defined!\n");
#else
	printf_semi("ERROR: __FPU_PRESENT not defined!\n");
#endif

#ifdef __FPU_USED
	printf_semi("__FPU_USED defined!\n");
#else
	printf_semi("ERROR: __FPU_USED not defined!\n");
#endif


	LED_PWM_Start();

	// Set D1 to red, half brightness.
	SetLEDColor(D1, orange, 50);
	StoreLEDColor(D1, red);
	StoreLEDColor(D2, yellow);
	StoreLEDColor(D3, green);
	StoreLEDColor(D4, purple);
	StoreLEDColor(D5, blue);

	SetLEDState(POWER, LED_STATE_PULSE);
	SetLEDState(RADIO, LED_STATE_PULSE);

	SetLEDState(D1, LED_STATE_PULSE);
	SetLEDState(D2, LED_STATE_PULSE);
	SetLEDState(D3, LED_STATE_PULSE);
	SetLEDState(D4, LED_STATE_PULSE);
	SetLEDState(D5, LED_STATE_PULSE);
	
	// Delay 3 sec before starting the test.
	HAL_Delay(3000);
	//	Diag_ButtonTest();

	//	Diag_BatteryMeterTest();


	// Screw around with PG14
	/*
	{
		printf_semi("Debugging PG14\n");

		GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_MEDIUM;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

		while (GetButtonState(BUTTON_S2, NULL) != BUTTON_HELD)
		{
			// Write S1 to PG14
			HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, (GetButtonState(BUTTON_S1, NULL) == BUTTON_HELD) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		}

		// Put it back.
		GPIO_InitStruct.Pin = GPIO_PIN_14;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI6;
		HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
		printf_semi("Debugging PG14 done.\n");
	}
	*/

	// Onboard SPI Test
	SetupIMU();
	IMUTest(IMU_ONBOARD);
	HAL_Delay(1);
	ConfigureIMU(IMU_ONBOARD);
	HAL_Delay(3000);

}

int app_main(void)
{
	/*
	// Check button state
	if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)
	{
		// Enable the timers for 10s
		HAL_Delay(10000);

	}
	*/
 	printf_semi("Goodnight!\n");

	SetLEDState(RADIO, LED_STATE_OFF);
	SetLEDState(POWER, LED_STATE_OFF);
	SetLEDState(D1, LED_STATE_OFF);
	SetLEDState(D2, LED_STATE_OFF);
	SetLEDState(D3, LED_STATE_OFF);
	SetLEDState(D4, LED_STATE_OFF);
	SetLEDState(D5, LED_STATE_OFF);
	LED_PWM_Stop();
	HAL_DeInit();
	while (1)
	{
		__WFE();
	}

}

/// Hardware Information

/// Clock Test
bool Diag_ClockTest(void)
{
// If HSI doesn't work, we'll never make it here.
	bool passFlag = true;

	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

// HSI Test (skip)
  printf_semi("Skipping HSI test...\n");


// LSE Test

  printf_semi("Testing LSE\n");
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_StatusTypeDef result = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if (result != HAL_OK)
  {
	  //  Clock switch failed.
	  printf_semi("### LSE test FAILED!\n");
	  passFlag = false;
  }

  printf_semi("\tHSI test PASSED\n");

// HSE Test

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  result = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if (result != HAL_OK)
  {
	  //  Clock switch failed.
	  printf_semi("### HSE test FAILED!\n");
	  passFlag = false;
  }
  printf_semi("\tHSE test PASSED\n");

  return passFlag;

}

/// LED Test

/// Button Test
bool Diag_ButtonTest(void)
{

	printf_semi("Button Test:\nPress S1 (PA0) and S2 (PI3)\n");

	// Pulse D1
	StoreLEDColor(D1, red);
	StoreLEDColor(D5, red);
	SetLEDState(D1, LED_STATE_BLINK);
	SetLEDState(D2, LED_STATE_OFF);
	SetLEDState(D3, LED_STATE_OFF);
	SetLEDState(D4, LED_STATE_OFF);
	SetLEDState(D5, LED_STATE_OFF);

	while (GetButtonState(BUTTON_S1, NULL) != BUTTON_HELD)
	{

	}

	printf_semi("\tS1 Pressed!\n");


	// Pulse D1
	StoreLEDColor(D1, green);
	StoreLEDColor(D5, red);
	SetLEDState(D1, LED_STATE_ON);
	SetLEDState(D2, LED_STATE_OFF);
	SetLEDState(D3, LED_STATE_OFF);
	SetLEDState(D4, LED_STATE_OFF);
	SetLEDState(D5, LED_STATE_BLINK);

	while (GetButtonState(BUTTON_S2, NULL) != BUTTON_HELD)
	{

	}

	printf_semi("\tS2 Pressed!\n");
	
	StoreLEDColor(D1, green);
	StoreLEDColor(D5, green);
	SetLEDState(D1, LED_STATE_ON);
	SetLEDState(D2, LED_STATE_OFF);
	SetLEDState(D3, LED_STATE_OFF);
	SetLEDState(D4, LED_STATE_OFF);
	SetLEDState(D5, LED_STATE_ON);

	HAL_Delay(2000);

	return true;
}

/// Battery Meter Test
bool Diag_BatteryMeterTest(void)
{

	float voltage = readBatteryLevel();

	// Printing floats is a big, stack-sucking deal.  Print as mV  
	printf_semi("Battery Level reading is %d\n", (uint32_t)(voltage * 1000));

	// Delay 3 sec before starting the test.
	HAL_Delay(3000);

	return true;

}

/// Onboard IMU Test

/// Radio Test

/// Off-board SPI test


/// Off-board I²C test

/// Sleep Test
 