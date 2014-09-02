#include <stm32f4xx_hal.h>
#include "Semihosting.h"

#ifdef __cplusplus
extern "C"
#endif

// Application States
typedef enum
{
	APP_HALT,
	APP_STANDBY,
	APP_RUN,
	APP_USB
} AppState;

AppState glove_state = APP_STANDBY;

/// Performs steps necessary to switch to a new state.
void app_switchstate(AppState state);

/// Check and service IMU interrupts
void CheckIMUInterrupts(void);

extern void initialise_monitor_handles(void);

void app_preinit(void)
{

	//


}

void app_postinit(void)
{

	// Startup state.

	// Play startup animation.

	// Initialize Wireless

	// Check USB

}

// Application Main loop
void app_main(void)
{
	initialise_monitor_handles();

	printf_semi("ZarthCode LLC\n");

	while (1)
	{
	// Common
		// USB Change?
		// Wireless connection change?
		// Charger connected?

		switch (glove_state)
		{
		case APP_RUN:

			// Check for new IMU raw data
				// Translate into scaled data
				// Process into quaternion frame using ahrs algo.

			// Check for newly completed quaternion frame
				// Process into skeletal data.

			// Check for newly completed skeletal data
				// Perform gesture recognition

			break;
		case APP_USB:
			// Clock at 168MHz
			break;

		default:
			// Aww man...
			printf_semi("Unrecognized glove AppState(%d)\n", glove_state);
			break;
		}


	}


}


/// Switch to specified application state.
void app_switchstate(AppState state)
{
	switch (state)
	{
	case APP_HALT:
		// LEDs red.

		// Disable radio

		// Switch to HSI clock
		
		// 
		break;
	case APP_STANDBY:
		// HSI Clock

		// Wake on external button (set as EXTI) or USB connection

		// Go to sleep

		// Welcome back!


		// Switch to run state
		glove_state = APP_RUN;	/// @todo check usb state first.

		// No break here.
	case APP_RUN:
		// Clock at full speed (180MHz)

		// 


		break;
	case APP_USB:
		// Clock at 168MHz
		break;

	default:
		// Aww man...
		printf_semi("Unrecognized glove AppState(%d)", glove_state);
		break;
	}

}


/// Check Interrupts
void CheckIMUInterrupts(void)
{


}
