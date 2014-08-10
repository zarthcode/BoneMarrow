// Button.h
/// @brief Button handling code
#include <stdint.h>

#define BUTTON_DEBOUNCE 20		// ms

/* Not implementing events, for now.
#define BUTTONEVENT_TIMEOUT	1000		// ms
#define BUTTONEVENT_CLICK_TIME 500		// ms
#define BUTTONEVENT_DBLCLICK_TIME 800	// ms
*/

/// button states
typedef enum
{

	BUTTON_RELEASED,	// Indicates that the button has been released for at least BUTTON_DEBOUNCE time.
//	BUTTON_UNPRESSED,	// Was formerly pressed, is now released, but not past debounce time
//	BUTTON_PRESSED,		// Was formerly released, is now pressed, but not past debounce time.
	BUTTON_HELD			// Button has been held for button's debounce time.

} ButtonStateType;


/// Buttons
typedef enum
{
	BUTTON_S1,
	BUTTON_S2,
	BUTTONS_TOTAL
} ButtonIDType;

/// @brief Returns (debounced) button state
/// @param button The identifier of the button to retrieve the state of.
/// @param time	Pointer to a variable to report the amount of time the state has been in the state.
/// @returns Current button state.
ButtonStateType GetButtonState(ButtonIDType button, uint16_t* time);

void UpdateButtonState(ButtonIDType button);

/// @brief 1ms Systick button handling code
void ButtonSystickHandler(void);

