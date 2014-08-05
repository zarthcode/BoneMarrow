#include "LED.h"
#include "tim.h"

/// LED States
LED_STATEType LEDStates[7] = {	LED_STATE_OFF,		// Power LED
								LED_STATE_OFF,		// Radio LED
								LED_STATE_OFF, LED_STATE_OFF, LED_STATE_OFF, LED_STATE_OFF, LED_STATE_OFF };	// RGB LEDs

/// Starting conditions for an LED in an animation.
ColorStruct RGBLED_Color[5] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

/// Ending condition for an LED in an animation.


/// Amount of time remaining in color transition
uint16_t TransitionTimeRemaining[7] = { 0, 0, 0, 0, 0, 0, 0 };
uint16_t TransitionTime[7] = { 0, 0, 0, 0, 0, 0, 0 };

/// Animation State
LED_ANIMATIONType animationState = LED_ANIM_NONE;


/// LED Animation Manager
void LED_Anim_Callback(void)
{

	// Update each LED
	for (int i = 0; i < LED_LAST; i++)
	{
		if (TransitionTimeRemaining[i] > 0)
		{
			switch (animationState)
			{
			case LED_ANIM_NONE:

				// Continue the LED state
				switch (LEDStates[i])
				{
				case LED_STATE_OFF:

					// Continue fade to black.
					
					break;

				case LED_STATE_ON:

					// Fade/Cycle to destination color
					
					break;

				case LED_STATE_BLINK:

					// At 0, toggle 

					break;

				case LED_STATE_PULSE:

					if (TransitionTimeRemaining[i] == LED_PULSE_SPEED)
					{
						// Start at black
						SetLEDColor(i, black, 0);
					}
					else if (TransitionTimeRemaining[i] == 0)
					{

						// Optionally end the animation?

						// Restart the pulse.
						TransitionTimeRemaining[i] = LED_PULSE_SPEED;

					}
			/*		
					else if (TransitionTimeRemaining[i] == (LED_PULSE_SPEED / 2))
					{
						// Max Brightness
						SetLEDColor((LED_IDType)i, RGBLED_Color[i - D1], 1.0f, 0);
					} 
			*/
					else if (TransitionTimeRemaining[i] < (LED_PULSE_SPEED / 2))
					{
						// Decreasing brightness
						SetLEDColor((LED_IDType)i, RGBLED_Color[i - D1], ((float)(TransitionTimeRemaining[i]) / (LED_PULSE_SPEED / 2.0f)));
					}
					else if (TransitionTimeRemaining[i] > (LED_PULSE_SPEED / 2))
					{
						// Increasing brightness
						SetLEDColor((LED_IDType)i, RGBLED_Color[i - D1], (1.0f - ((float)(TransitionTimeRemaining[i] - (LED_PULSE_SPEED / 2.0f)) / (LED_PULSE_SPEED/2.0f))));
					}

					break;

				default:
					break;
				}

				break;

			case LED_ANIM_FUEL:

				/// TODO - Unimplemented.
				break;

			case LED_ANIM_TWINKLE:
			case LED_ANIM_BOUNCE:
			case LED_ANIM_CYCLE:
				break;

			default:

				break;
			}


			// Decrement the timer
			TransitionTimeRemaining[i]--;

			if (TransitionTimeRemaining[i] == 0)
				TransitionTimeRemaining[i] = TransitionTime[i];

		}


	}

}

/// Sets the destination color for an LED
void SetLEDColor(LED_IDType led, ColorStruct color, float brightness)
{

	TIM_HandleTypeDef* htim = NULL;

	switch (led)
	{
	case POWER:
		// Ignore color just set as scaled brightness
		// Set pwm pulse width.
		htim1.Instance->CCR4 = LED_MAX * (brightness / MAX_BRIGHTNESS);
		return;
		break;

	case RADIO:
		// Ignore color just set as scaled brightness
		// Set pwm pulse width.
		htim3.Instance->CCR4 = LED_MAX * (brightness / MAX_BRIGHTNESS);
		return;
		break;

	case D1:

		// Set timer handle
		htim = &htim3;
		break;

	case D2:

		// Set timer handle
		htim = &htim8;
		break;
	
	case D3:

		// Set timer handle
		htim = &htim4; 
		break;
	
	case D4:

		// Set timer handle
		htim = &htim1;
		break;
	
	case D5:

		// Set timer handle
		htim = &htim5;
		
		break;

	default:
		// Assert - Invalid LED
		break;
	}

	
	// Set R, G, and B elements.
	// Set pwm pulse width.
	float intensity = color.Red + color.Green + color.Blue;

	// R
	htim->Instance->CCR2 = (uint32_t)(LED_MAX * ((float)color.Red / LED_MAX) * (brightness * MAX_BRIGHTNESS) * 3.0f * ((float)color.Red / intensity));

	// G
	htim->Instance->CCR1 = (uint32_t)(LED_MAX * ((float)color.Green / LED_MAX) * (brightness * MAX_BRIGHTNESS) * 3.0f * ((float)color.Green / intensity));

	// B
	htim->Instance->CCR3 = (uint32_t)(LED_MAX * ((float)color.Blue / LED_MAX) * (brightness * MAX_BRIGHTNESS) * 3.0f * ((float)color.Blue / intensity));

}


/// Stores the Destination color for an LED
void StoreLEDColor(LED_IDType led, ColorStruct color)
{

	// Store LED color
	RGBLED_Color[led - D1] = color;

}

/// Sets the behavior of an individual LED
void SetLEDState(LED_IDType led, LED_STATEType state)
{
	// Setup the transition
	switch (state)
	{
	case LED_STATE_OFF:
		// Switch the LED's brightness channel to 0%
		SetLEDColor(led, black, 0);

		break;
	case LED_STATE_ON:

		break;

	case LED_STATE_BLINK:

		break;

	case LED_STATE_PULSE:
		// Setup transition
		TransitionTime[led] = LED_PULSE_SPEED;
		TransitionTimeRemaining[led] = LED_PULSE_SPEED;
		break;

	default:
		// Do nothing.
		break;
	}

	LEDStates[led] = state;

}


/// Sets the current animation
void SetLED_Animation(LED_ANIMATIONType animation)
{


}


/// LED PWM Start
void LED_PWM_Start(void)
{
	// activate the PWM Timers outputs

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

	// Activate the advanced timers
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_3);


}

/// LED PWM Stop
void LED_PWM_Stop(void)
{

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