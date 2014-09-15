/// @file SPIManager.h SPI DMA Transfer Manager
#include "stm32f4xx_hal.h"

/// SPI Operating Modes
typedef enum
{

	UTIL_SPI_OPMODE_TXRX,
	UTIL_SPI_OPMODE_TX,
	UTIL_SPI_OPMODE_RX

} UTIL_SPI_OPERATING_MODEType;

/// Utility Function to enable Change SPI operating modes
void UTIL_SPI_SetOperatingMode(SPI_HandleTypeDef* hspi, UTIL_SPI_OPERATING_MODEType mode);