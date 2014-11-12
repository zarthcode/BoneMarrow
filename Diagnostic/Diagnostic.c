#include <stm32f4xx_hal.h>
#include <stdint.h>
#include <stdbool.h>		// C99 req'd.
#include "Semihosting.h"
#include "Button.h"
#include "tim.h"
#include "LED.h"
#include "IMU.h"
#include "Battery.h"
#include "Delay.h"
#include "WiFi.h"

extern void initialise_monitor_handles(void);
bool Diag_ButtonTest(void);
bool Diag_ClockTest(void);
bool Diag_BatteryMeterTest(void);
bool Diag_IMUTest(void);
bool Diag_WiFi(void);

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

	Delay_Init();		// Initialize uS delay routine.


	/*

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
	*/
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
//	IMU_Test(IMU_ONBOARD);
	/// \todo Diagnostic.c needs to call self-test routines, then performance/eval/diag routines and provide structured output.
/*	printf_semi("IMU Diagnostic...");
	Diag_IMUTest() ? printf_semi("PASSED\n") : printf_semi("FAILED\n");
	Diag_IMUFrameTest();

*/

//	HAL_Delay(3000);

	// WiFi Initialization and Self-test
	printf_semi("WiFi Diagnostic...");
	Diag_WiFi() ? printf_semi("PASSED\n") : printf_semi("FAILED\n");

}

extern uint32_t volatile IMU_framecount;

int app_main(void)
{

	printf_semi("Finished (10s wait)\n");
	HAL_Delay(10000);

	// Shutdown


 	printf_semi("Going to sleep!\n");
	SetLEDState(RADIO, LED_STATE_OFF);
	SetLEDState(POWER, LED_STATE_OFF);
	SetLEDState(D1, LED_STATE_OFF);
	SetLEDState(D2, LED_STATE_OFF);
	SetLEDState(D3, LED_STATE_OFF);
	SetLEDState(D4, LED_STATE_OFF);
	SetLEDState(D5, LED_STATE_OFF);
	LED_PWM_Stop();
	IMU_Disable();

	/// @todo A device state manager to turn devices on/off and change processor modes...
	HAL_SPI_DeInit(&hspi1);
	HAL_DeInit();
	while (1)
	{
		__WFE();
	}
}

/// WiFi Self-Test
bool Diag_WiFi(void)
{
	// Start wifi
	WiFi_Configure();


	return true;
}

/// IMU Frame Test
bool Diag_IMUFrameTest(void)
{
	printf_semi("Waiting for complete frame input.\n");
	IMU_Enable();

	uint32_t oldfc = IMU_framecount;


	// Process frame data for a few secs...
//	uint32_t startTime = HAL_GetTick();
//	while ((startTime + 1000) > HAL_GetTick())
	while (IMU_framecount < 100)
	{
		IMU_ProcessRAWFrame();		// Scale RAW into scaled data (minimum processing.)
	}
		// Process Framedata
	IMU_ProcessRAWFrame();		// Scale RAW into scaled data (minimum processing.)

	for (int imu = 0; imu < IMU_LAST; imu++)
	{
		DIAG_IMU_Test(imu);
	}

		// Update Gesture Recognition

		// Measure Performance

			// Check for idle hands...

			// Adjust buffer sizes

			// Adjust full-scale settings

			// Readjust ODRs


	//}
	
	printf_semi("RAW Frames Generated: %d\n", IMU_framecount);
	printf_semi("SCALED Frames processed: %d\n", SPATIAL_IMUFrameBuffer_NumProcessed);
	for (int imu = 0; imu < IMU_LAST; imu++)
	{
	// Print RAW data
		printf_semi("IMU %d\n", imu);
	printf_semi("\tRAW:\tax = %d ay = %d az = %d\n\t\tgx = %d gy = %d gz = %d\n",
		IMU_RAWFramebuffer[0].imu[imu].accelerometer.x,
		IMU_RAWFramebuffer[0].imu[imu].accelerometer.y,
		IMU_RAWFramebuffer[0].imu[imu].accelerometer.z,
		IMU_RAWFramebuffer[0].imu[imu].gyroscope.x,
		IMU_RAWFramebuffer[0].imu[imu].gyroscope.z);

	// Print scaled data
	printf_semi("\tScaled:\tax = %f ay = %f az = %f\n\t\tgx = %f gy = %f gz = %f\n",
		SPATIAL_IMUFrameBuffer[0].imu[imu].accelerometer.x,
		SPATIAL_IMUFrameBuffer[0].imu[imu].accelerometer.y,
		SPATIAL_IMUFrameBuffer[0].imu[imu].accelerometer.z,
		SPATIAL_IMUFrameBuffer[0].imu[imu].gyroscope.x,
		SPATIAL_IMUFrameBuffer[0].imu[imu].gyroscope.z);

	// Print calculated quaternion
	printf_semi("\tQuaternion:\n\t\tqw = %f qx = %f qy = %f qz = %f\n",
		SPATIAL_QUATERNION_Framebuffer[0].q[imu].w,
		SPATIAL_QUATERNION_Framebuffer[0].q[imu].x,
		SPATIAL_QUATERNION_Framebuffer[0].q[imu].y,
		SPATIAL_QUATERNION_Framebuffer[0].q[imu].z);
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
//	DIAG_IMU_CheckAlignment();

	// Initialize SPI Structures
	IMU_Setup();
	
	// Check IMU MISO/MOSI/SCK connectivity
	// Perform 100 tests
	int testresults[IMU_LAST][1+(IMU_SUBDEV_LAST*2)];

	for (int imu = 0; imu < IMU_LAST; imu++)
	{
		for (int idx = 0; idx < (1 + (IMU_SUBDEV_LAST * 2)); idx++)
		{
			testresults[imu][idx] = 0;
		}
	}

	printf_semi("This test must be run after a cold boot.\n");
	printf_semi("Checking IMU SPI Connectivity...\n");
	for (int num = 0; num < 100; num++)
	{
		for (int imu = 0; imu < IMU_LAST; imu++)
		{
			if (DIAG_IMU_Test(imu))
			{
				testresults[imu][0]++;
				//	printf_semi(".");	// DIAG_IMU_Test() can be chatty enough...
			}

		}
	}


	// On powerup, ALL IMU Interrupts should be LOW
	for (IMU_IDType imu = 0; imu < IMU_LAST; imu++)
	{
		for (IMU_SubDeviceType subdev = 0; subdev < IMU_SUBDEV_LAST; subdev++)
		{
			// Evaluate the interrupt status
			testresults[imu][subdev+1] = (GPIO_PIN_SET == HAL_GPIO_ReadPin(IMUDevice[imu].INTMappings[subdev].port, IMUDevice[imu].INTMappings[subdev].pin)) ? true : false;
		}
	}


	for (IMU_IDType imu = 0; imu < IMU_LAST; imu++)
	{
		IMU_Configure(imu);		// Setup the IMU for standard interrupt notifications, etc.
	}

	HAL_Delay(100);	// Wait 100mS for data to show up.

	for (IMU_IDType imu = 0; imu < IMU_LAST; imu++)
	{
		for (IMU_SubDeviceType subdev = 0; subdev < IMU_SUBDEV_LAST; subdev++)
		{
			// Evaluate the interrupt status
			testresults[imu][subdev + IMU_SUBDEV_LAST + 1] = (GPIO_PIN_SET == HAL_GPIO_ReadPin(IMUDevice[imu].INTMappings[subdev].port, IMUDevice[imu].INTMappings[subdev].pin)) ? true : false;
		}
	}

	// Print results/evaluation
	for (IMU_IDType imu = 0; imu < IMU_LAST; imu++)
	{
		printf_semi("IMU[%d]: Connectivity %d%%, ", imu, testresults[imu][0]);
		for (int subdev = 0; subdev < IMU_SUBDEV_LAST; subdev++)
		{
			switch (subdev)
			{
			case IMU_SUBDEV_ACC:
				printf_semi("INT_A ");
				break;
			case IMU_SUBDEV_GYRO:
				printf_semi("INT_G ");
				break;
			default:
				printf_semi("\n\nUnknown Subdev. UPDATE TEST!\n\n");
				return false;
				break;
			}

			if ((testresults[imu][subdev + 1] == false) && (testresults[imu][subdev + 1 + IMU_SUBDEV_LAST] == true))
				printf_semi("PASS ");
			else if ((testresults[imu][subdev + 1] == false) && (testresults[imu][subdev + 1 + IMU_SUBDEV_LAST] == false))
				printf_semi("FAIL(OPEN/DISC) ");
			else if ((testresults[imu][subdev + 1] == true) && (testresults[imu][subdev + 1 + IMU_SUBDEV_LAST] == false))
				printf_semi("FAIL(INVERTED?) ");
			else // if ((testresults[imu][subdev + 1] == true) && (testresults[imu][subdev + 1 + IMU_SUBDEV_LAST] == true))
				printf_semi("FAIL(SHORT/ALREADYON) ");

		}
		printf_semi("\n");

	}

	// If anything failed, the test failed.
	bool teststatus = true;

	for (IMU_IDType imu = 0; imu < IMU_LAST; imu++)
	{

		if ((testresults[imu][0] < 100) || !((testresults[imu][1] == false) && (testresults[imu][2] == true)))
		{
			return false;
		}


	}

}
/// Radio Test

/// Off-board SPI test

/// SPI1 Workaround Test


/// IMU Zero Calibration Test

/// BoneMarrow Test
	// Acquire frame
	// Translate to Scaled data
	// Translate to Relative data
	// Translate to Quaternion data

/// Off-board I²C test

/// Sleep Test
 