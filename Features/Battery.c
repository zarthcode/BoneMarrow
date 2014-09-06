#include "Battery.h"
#include "adc.h"
#include "Semihosting.h"

/// @todo Add a calibration function that stores data to eeprom.

/// @brief Read the ADC battery level
/// @returns Battery voltage, (In volts. accurate to 12bits.)
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
		printf_semi("Unknown HAL return code.\n");
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
	/// @TODO - Clean this up, replace non-calibrated variables w/#defines
	/// @TODO - Zener Voltage Calibration values from EEPROM
	float Vzener = 2.58f;

	// R20 is 68 Ohm
	float R20 = 68.0f;

	// R22 is 240 Ohm
	float R22 = 240.0f;

	// Vref is 3.3V (buck-boost supply voltage. Accurate enough.)
	float Vref = 3.3f;
	
	// Conversion is a 12-bit number, referenced from 0 to Vref.
	float ADCVoltage = ConversionResult * (Vref / 4096);
	
	float BatteryVoltage = ((ADCVoltage * (R20 + R22)) / R22) + Vzener;

	// Work-around for printing floats
	printf_semi("Battery Level reading is %f V (Raw reading: %f)\n", BatteryVoltage, ConversionResult);

	return BatteryVoltage;
}