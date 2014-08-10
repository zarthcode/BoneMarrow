// Button.c
// External Button Implementation
#include "Button.h"
#include <stm32f4xx_hal.h>
#include <stdbool.h>

// State variables
ButtonStateType ButtonStates[BUTTONS_TOTAL] = { 0, 0 };
uint16_t Button_SetTime[BUTTONS_TOTAL] = { 0, 0 };
uint16_t Button_ResetTime[BUTTONS_TOTAL] = { 0, 0 };

ButtonStateType GetButtonState(ButtonIDType button, uint16_t* time)
{
	if (time)
	{
		*time = ButtonStates[button] == BUTTON_RELEASED ? Button_ResetTime[button] : Button_SetTime[button];
	}
	return ButtonStates[button];

}



void ButtonSystickHandler(void)
{
	/// @todo 1ms is probably too often for this.
	UpdateButtonState(BUTTON_S1);
	UpdateButtonState(BUTTON_S2);
	
}

void UpdateButtonState(ButtonIDType button)
{
	GPIO_TypeDef* port;
	uint16_t pin;


	// Button variables
	switch (button)
	{
	case BUTTON_S1:
		port = GPIOA;
		pin = GPIO_PIN_0;
		break;

	case BUTTON_S2:
		port = GPIOI;
		pin = GPIO_PIN_3;
		break;

	default:
		// @fail
		break;
	}

// Update button
	// Debounce button state.
	if (HAL_GPIO_ReadPin(port, pin) == GPIO_PIN_SET)
	{
		// Increment high counter
		if (Button_SetTime[button] != UINT16_MAX)		// An overflow would be kinda tragic.
			Button_SetTime[button]++;
		
		Button_ResetTime[button] = 0;
	}
	else
	{
		if (Button_ResetTime[button] != UINT16_MAX)	
			Button_ResetTime[button]++;

		Button_SetTime[button] = 0;
	}

	// Compare to the button state.
	switch (ButtonStates[button])
	{
	case BUTTON_RELEASED:
		// Indicates that the button has been released for at least BUTTON_DEBOUNCE time.
		if (Button_SetTime[button] > BUTTON_DEBOUNCE)
		{
			// Button is now pressed.
			ButtonStates[button] = BUTTON_HELD;
			
		}
		break;

	case BUTTON_HELD:
		// Button has been held for button's debounce time.
		if (Button_ResetTime[button] > BUTTON_DEBOUNCE)
		{
			// Button is now released.
			ButtonStates[button] = BUTTON_RELEASED;
			
		}

		break;
	default:

		// @fail
		break;
	}

}

