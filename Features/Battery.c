#include "Battery.h"
#include "adc.h"
#include <stdio.h>


// Read the ADC battery level
float readBatteryLevel(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;

	// Sink pin PC1
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
	
	// Read voltage on ADC1_IN0 / PC0
	// Start the conversion
	HAL_ADC_Start(&hadc1);
	HAL_StatusTypeDef result = HAL_ADC_PollForConversion(&hadc1, 50);
	uint32_t ConversionResult = 0.0f;
	switch(result)
	{
	case HAL_TIMEOUT:
		ConversionResult = 0.0f;
		break;

	case HAL_OK:
		ConversionResult = HAL_ADC_GetValue(&hadc1);
		break;

	default:
		// Unimplemented HAL result
#ifdef DEBUG
		printf("Unknown HAL return code.\n");
#endif
		break;
	}
	
	HAL_ADC_Stop(&hadc1);


	// Bring PC1 to Hi-Z
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	// Determine the battery voltage.

	// Zener voltage is ~2.7V
	// R20 is 68 Ohm
	// R22 is 240 Ohm

	/// @TODO - Zener Voltage Calibration values from EEPROM
	float BatteryVoltage = 1.283f * ((float)ConversionResult / 1000.0f) + 2.7f;

#ifdef DEBUG
	printf("Battery Level reading is %d mV (Raw reading: %d)\n", (uint32_t)(BatteryVoltage * 1000), ConversionResult);
#endif

	return BatteryVoltage;
}