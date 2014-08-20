/// @file Implementation of LSM330DLC routines.
#include "LSM330DLC.h"

uint8_t LSM330DLC_FormatAddress(bool read, bool increment, uint8_t address)
{

	return (read ? IMU_SPI_READ : IMU_SPI_WRITE) | (increment ? IMU_SPI_AUTOINCREMENT : 0x00) | (address & 0x3F);

}
