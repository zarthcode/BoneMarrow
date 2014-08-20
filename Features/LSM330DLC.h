/// @file LSM330DLC Register defines and functions
#include <stdint.h>
#include <stdbool.h>

// LSM330DLC_ Namespace

// LSM330DLC format
#define IMU_SPI_READ			(0x80)
#define IMU_SPI_WRITE			(0x00)
#define IMU_SPI_AUTOINCREMENT	(0x40)

#define LSM330_ADDRESS_MASK		(0x3F)		// 5-bit address mask

/**
 * @brief Low level routine that returns a byte that encodes the register address, auto-increment, and rw options
 * @returns uint8_t encoded byte to be sent. (MSB first).
 * @param bool read TRUE if this is a read operation.  False, otherwise.
 * @param bool increment TRUE if subsequently reading/writing multiple bytes of data using this one command.
 * @param uint8_t address Register address to begin reading/writing from/to.
 */
uint8_t LSM330DLC_FormatAddress(bool read, bool increment, uint8_t address);

// LSM330DLC Registers - (From datasheet)
#define LSM330DLC__CTRL_REG1_A		(0x20)
#define LSM330DLC__CTRL_REG2_A		(0x21)
#define LSM330DLC__CTRL_REG3_A		(0x22)
#define	LSM330DLC__CTRL_REG4_A		(0x23)
#define	LSM330DLC__CTRL_REG5_A		(0x24)
#define LSM330DLC__CTRL_REG6_A		(0x25)
#define LSM330DLC__REFERENCE_A		(0x26)
#define LSM330DLC__STATUS_REG_A		(0x27)
#define LSM330DLC__OUT_X_L_A		(0x28)
#define LSM330DLC__OUT_X_H_A		(0x29)
#define LSM330DLC__OUT_Y_L_A		(0x2A)
#define LSM330DLC__OUT_Y_H_A		(0x2B)
#define LSM330DLC__OUT_Z_L_A		(0x2C)
#define LSM330DLC__OUT_Z_H_A		(0x2D)
#define LSM330DLC__FIFO_CTRL_REG	(0x2E)
#define LSM330DLC__FIFO_SRC_REG		(0x2F)
#define LSM330DLC__INT1_CFG_A		(0x30)
#define LSM330DLC__INT1_SOURCE_A	(0x31)
#define LSM330DLC__INT1_THS_A		(0x32)
#define LSM330DLC__INT1_DURATION_A	(0x33)
#define LSM330DLC__INT2_CFG_A		(0x34)
#define LSM330DLC__INT2_SOURCE_A	(0x35)
#define LSM330DLC__INT2_THS_A		(0x36)
#define LSM330DLC__INT2_DURATION_A	(0x37)
#define LSM330DLC__CLICK_CFG_A		(0x38)
#define LSM330DLC__CLICK_SRC_A		(0x39)
#define LSM330DLC__CLICK_THS_A		(0x3A)
#define LSM330DLC__TIME_LIMIT_A		(0x3B)
#define LSM330DLC__TIME_LATENCY_A	(0x3C)
#define LSM330DLC__TIME_WINDOW_A	(0x3D)
#define LSM330DLC__Act_THS			(0x3E)
#define LSM330DLC__Act_DUR			(0x3F)
#define LSM330DLC__WHO_AM_I_G		(0x0F)
#define LSM330DLC__CTRL_REG1_G		(0x20)
#define LSM330DLC__CTRL_REG2_G		(0x21)
#define LSM330DLC__CTRL_REG3_G		(0x22)
#define LSM330DLC__CTRL_REG4_G		(0x23)
#define LSM330DLC__CTRL_REG5_G		(0x24)
#define LSM330DLC__REFERENCE_G		(0x25)
#define LSM330DLC__OUT_TEMP_G		(0x26)
#define LSM330DLC__STATUS_REG_G		(0x27)
#define LSM330DLC__OUT_X_L_G		(0x28)
#define LSM330DLC__OUT_X_H_G		(0x29)
#define LSM330DLC__OUT_Y_L_G		(0x2A)
#define LSM330DLC__OUT_Y_H_G		(0x2B)
#define LSM330DLC__OUT_Z_L_G		(0x2C)
#define LSM330DLC__OUT_Z_H_G		(0x2D)
#define LSM330DLC__FIFO_CTRL_REG_G	(0x2E)
#define LSM330DLC__FIFO_SRC_REG_G	(0x2F)
#define LSM330DLC__INT1_CFG_G		(0x30)
#define LSM330DLC__INT1_SRC_G		(0x31)
#define LSM330DLC__INT1_TSH_XH_G	(0x32)
#define LSM330DLC__INT1_TSH_XL_G	(0x33)
#define LSM330DLC__INT1_TSH_YH_G	(0x34)
#define LSM330DLC__INT1_TSH_YL_G	(0x35)
#define LSM330DLC__INT1_TSH_ZH_G	(0x36)
#define LSM330DLC__INT1_TSH_ZL_G	(0x37)
#define LSM330DLC__INT1_DURATION_G	(0x38)
