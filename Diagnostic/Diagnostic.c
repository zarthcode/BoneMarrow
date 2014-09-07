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
bool Diag_IMUTest(void);

int app_preinit(void)
{

	
}

int app_postinit(void)
{

#ifdef DEBUG
	HAL_EnableDBGSleepMode();
	HAL_EnableDBGStandbyMode();
	HAL_EnableDBGStopMode();

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

//	IMU_CheckAlignment();

	// Check PG10
	printf_semi("PG10 IMU_ONBOARD - INT1_A is ");
	if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == GPIO_PIN_SET)
		printf_semi("HIGH\n");
	else
		printf_semi("LOW\n");

	// Onboard SPI Test
	IMU_Setup();
//	IMU_Test(IMU_ONBOARD);

	IMU_Configure(IMU_ONBOARD);

//	HAL_Delay(3000);

}

extern uint32_t volatile IMU_framecount;

int app_main(void)
{
	printf_semi("Waiting for input.\n");

	uint32_t oldfc = IMU_framecount;


	// Process frame data for a few secs...
//	uint32_t startTime = HAL_GetTick();
//	while ((startTime + 1000) > HAL_GetTick())
	while (IMU_framecount == 0)
	{
	}
		// Process Framedata
	IMU_ProcessRAWFrame();		// Scale RAW into scaled data (minimum processing.)
	IMU_ProcessOrientation();	// Process Framedata into Quaternions


		// Update Gesture Recognition

		// Measure Performance

			// Check for idle hands...

			// Adjust buffer sizes

			// Adjust full-scale settings

			// Adjust ODRs


	//}
	
	printf_semi("RAW Frames Generated: %d\n", IMU_framecount);
	printf_semi("SCALED Frames processed: %d\n", SPATIAL_IMUFrameBuffer_NumProcessed);

	printf_semi("Last Frame:\nax = %d\nay = %d\naz = %d\ngx = %d\ngy = %d\ngz = %d\n",
		IMU_RAWFramebuffer[0].imu[0].accelerometer.x,
		IMU_RAWFramebuffer[0].imu[0].accelerometer.y,
		IMU_RAWFramebuffer[0].imu[0].accelerometer.z,
		IMU_RAWFramebuffer[0].imu[0].gyroscope.x,
		IMU_RAWFramebuffer[0].imu[0].gyroscope.z);

	printf_semi("Last Frame:\nax = %f\nay = %f\naz = %f\ngx = %f\ngy = %f\ngz = %f\n",
		SPATIAL_IMUFrameBuffer[0].imu[0].accelerometer.x,
		SPATIAL_IMUFrameBuffer[0].imu[0].accelerometer.y,
		SPATIAL_IMUFrameBuffer[0].imu[0].accelerometer.z,
		SPATIAL_IMUFrameBuffer[0].imu[0].gyroscope.x,
		SPATIAL_IMUFrameBuffer[0].imu[0].gyroscope.z);


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
bool Diag_IMUTest(void)
{
	/// @todo This needs to be an entire function that examines that the compiler has correctly packed the variables together.
	DIAG_IMU_CheckAlignment();

	// Onboard SPI Test
	IMU_Setup();

	DIAG_IMU_Test(IMU_ONBOARD);
	HAL_Delay(1);

	/// @todo This needs to be done while the IMU is not configured.
	printf_semi("PG10 IMU_ONBOARD - INT1_A is ");
	if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == GPIO_PIN_SET)
		printf_semi("HIGH\n");
	else
		printf_semi("LOW\n");
	
	IMU_Configure(IMU_ONBOARD);

	// It should take awhile for a sample to appear...
	printf_semi("PG10 IMU_ONBOARD - INT1_A is ");
	if (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == GPIO_PIN_SET)
		printf_semi("HIGH\n");
	else
		printf_semi("LOW\n");

	HAL_Delay(100);

	/// @todo Verify the interrupt using a polling operation
// 	IMU_Poll(IMU_ONBOARD, IMU_SUBDEV_ACC);
	IMU_CheckIMUInterrupts();


	/// @todo report success or failure.
	HAL_Delay(3000);


}

/// Radio Test

/// Off-board SPI test

/// IMU Zero Calibration Test

/// BoneMarrow Test
	// Acquire frame
	// Translate to Scaled data
	// Translate to Relative data
	// Translate to Quaternion data

/// Off-board I²C test

/// Sleep Test
 