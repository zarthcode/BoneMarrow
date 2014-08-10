/*

LED locations:

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
#include <stdint.h>

// LED speed 
#define LED_PULSE_SPEED 1000 /* mS */
#define LED_BLINK_SPEED 125 /* mS */
#define LED_FAST_BLINK_SPEED 85 /* mS */
#define MAX_BRIGHTNESS 0.25f /* % */

#define LED_MAX 250
#define PWM_PERIOD 1000

// LED States

typedef enum
{
	LED_STATE_OFF = 0,		// OFF/Zero brightness. (Timer disabled.)
	LED_STATE_ON,			// Leave the LED on at a specified color/brightness. (No animation)
	LED_STATE_BLINK,		// Blink at a pre-determined speed.
	LED_STATE_PULSE			// Gently pulse the LED brightness (req. processor time)

} LED_STATEType;

/// Animation States
/// @brief Animations utilize multiple LEDs to display data, overriding their contents/states.
typedef enum
{

	LED_ANIM_NONE = 0,		// No animation / Disabled
	LED_ANIM_FUEL,			// Fuel-gauge
	LED_ANIM_TWINKLE,		// Random twinkling lights
	LED_ANIM_BOUNCE,		// Bounce a color back and forth
	LED_ANIM_CYCLE,			// Cycle through the color wheel
} LED_ANIMATIONType;

/// LEDs
typedef enum
{
	POWER = 0,
	RADIO,
	D1,
	D2,
	D3,
	D4,
	D5,
	LED_LAST
} LED_IDType;

/// RGB LED Device
typedef struct
{

	uint32_t Red;
	uint32_t Green;
	uint32_t Blue;

} ColorStruct;

// Some prebuilt colors

static ColorStruct black =	{	0,				0,				0 };
static ColorStruct white = { LED_MAX, LED_MAX, LED_MAX };

static ColorStruct red =	{	LED_MAX,		0,				0 };
static ColorStruct orange = { LED_MAX, LED_MAX / 2, 0 };
static ColorStruct yellow = { LED_MAX, LED_MAX, 0 };
static ColorStruct green =	{	0,				LED_MAX,		0 };
static ColorStruct cyan = { 0, LED_MAX, LED_MAX };
static ColorStruct blue =	{	0,				0,				LED_MAX };
static ColorStruct purple = { LED_MAX / 2, 0, LED_MAX };
static ColorStruct magenta = { LED_MAX, 0, LED_MAX };

/// Sets the current color for an LED
void SetLEDColor(LED_IDType led, ColorStruct color, float brightness);

/// Stores the Destination color for an LED
void StoreLEDColor(LED_IDType led, ColorStruct color);

/// Sets the behavior of an individual LED
void SetLEDState(LED_IDType led, LED_STATEType state);

/// Sets the current animation
void SetLED_Animation(LED_ANIMATIONType animation);

/// LED 1ms callback handler
void LED_Anim_Callback(void);

/// LED PWM Start
void LED_PWM_Start(void);

/// LED PWM Stop
void LED_PWM_Stop(void);