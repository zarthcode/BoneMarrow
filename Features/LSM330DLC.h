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
 * @bug this needs to be accessed indirectly, via a switch.
 */
uint8_t LSM330DLC_FormatAddress(bool read, bool increment, uint8_t address);

// LSM330DLC Registers - (From datasheet)
#define LSM330DLC_REG_CTRL_REG1_A		(0x20)
#define LSM330DLC_REG_CTRL_REG2_A		(0x21)
#define LSM330DLC_REG_CTRL_REG3_A		(0x22)
#define	LSM330DLC_REG_CTRL_REG4_A		(0x23)
#define	LSM330DLC_REG_CTRL_REG5_A		(0x24)
#define LSM330DLC_REG_CTRL_REG6_A		(0x25)
#define LSM330DLC_REG_REFERENCE_A		(0x26)
#define LSM330DLC_REG_STATUS_REG_A		(0x27)
#define LSM330DLC_REG_OUT_X_L_A			(0x28)
#define LSM330DLC_REG_OUT_X_H_A			(0x29)
#define LSM330DLC_REG_OUT_Y_L_A			(0x2A)
#define LSM330DLC_REG_OUT_Y_H_A			(0x2B)
#define LSM330DLC_REG_OUT_Z_L_A			(0x2C)
#define LSM330DLC_REG_OUT_Z_H_A			(0x2D)
#define LSM330DLC_REG_FIFO_CTRL_REG	 	(0x2E)
#define LSM330DLC_REG_FIFO_SRC_REG		(0x2F)
#define LSM330DLC_REG_INT1_CFG_A		(0x30)	/// A write operation of this address is possible only after system boot.
#define LSM330DLC_REG_INT1_SOURCE_A	 	(0x31)
#define LSM330DLC_REG_INT1_THS_A		(0x32)
#define LSM330DLC_REG_INT1_DURATION_A	(0x33)
#define LSM330DLC_REG_INT2_CFG_A		(0x34)
#define LSM330DLC_REG_INT2_SOURCE_A	 	(0x35)
#define LSM330DLC_REG_INT2_THS_A		(0x36)
#define LSM330DLC_REG_INT2_DURATION_A	(0x37)
#define LSM330DLC_REG_CLICK_CFG_A		(0x38)
#define LSM330DLC_REG_CLICK_SRC_A		(0x39)
#define LSM330DLC_REG_CLICK_THS_A		(0x3A)
#define LSM330DLC_REG_TIME_LIMIT_A		(0x3B)
#define LSM330DLC_REG_TIME_LATENCY_A	(0x3C)
#define LSM330DLC_REG_TIME_WINDOW_A		(0x3D)
#define LSM330DLC_REG_Act_THS			(0x3E)
#define LSM330DLC_REG_Act_DUR			(0x3F)
#define LSM330DLC_REG_WHO_AM_I_G		(0x0F)
#define LSM330DLC_REG_CTRL_REG1_G		(0x20)
#define LSM330DLC_REG_CTRL_REG2_G		(0x21)
#define LSM330DLC_REG_CTRL_REG3_G		(0x22)
#define LSM330DLC_REG_CTRL_REG4_G		(0x23)
#define LSM330DLC_REG_CTRL_REG5_G		(0x24)
#define LSM330DLC_REG_REFERENCE_G		(0x25)
#define LSM330DLC_REG_OUT_TEMP_G		(0x26)
#define LSM330DLC_REG_STATUS_REG_G		(0x27)
#define LSM330DLC_REG_OUT_X_L_G			(0x28)
#define LSM330DLC_REG_OUT_X_H_G			(0x29)
#define LSM330DLC_REG_OUT_Y_L_G	 		(0x2A)
#define LSM330DLC_REG_OUT_Y_H_G	 		(0x2B)
#define LSM330DLC_REG_OUT_Z_L_G	 		(0x2C)
#define LSM330DLC_REG_OUT_Z_H_G	 		(0x2D)
#define LSM330DLC_REG_FIFO_CTRL_REG_G	(0x2E)
#define LSM330DLC_REG_FIFO_SRC_REG_G	(0x2F)
#define LSM330DLC_REG_INT1_CFG_G		(0x30)
#define LSM330DLC_REG_INT1_SRC_G		(0x31)
#define LSM330DLC_REG_INT1_TSH_XH_G		(0x32)
#define LSM330DLC_REG_INT1_TSH_XL_G	 	(0x33)
#define LSM330DLC_REG_INT1_TSH_YH_G	 	(0x34)
#define LSM330DLC_REG_INT1_TSH_YL_G	 	(0x35)
#define LSM330DLC_REG_INT1_TSH_ZH_G	 	(0x36)
#define LSM330DLC_REG_INT1_TSH_ZL_G	 	(0x37)
#define LSM330DLC_REG_INT1_DURATION_G	(0x38)

// ********************
// CTRL_REG1_A

// LSM330DLC Data rate
#define LSM330DLC_CTRL_REG1_A_ODR_POWERDOWN					(0x00)
#define LSM330DLC_CTRL_REG1_A_ODR_1HZ						(0x10)
#define LSM330DLC_CTRL_REG1_A_ODR_10HZ						(0x20)
#define LSM330DLC_CTRL_REG1_A_ODR_25HZ						(0x30)
#define LSM330DLC_CTRL_REG1_A_ODR_50HZ						(0x40)
#define LSM330DLC_CTRL_REG1_A_ODR_100HZ						(0x50)
#define LSM330DLC_CTRL_REG1_A_ODR_200HZ						(0x60)
#define LSM330DLC_CTRL_REG1_A_ODR_400HZ						(0x70)
#define LSM330DLC_CTRL_REG1_A_ODR_LP1620HZ					(0x80)
#define LSM330DLC_CTRL_REG1_A_ODR_NORMAL1344HZ_LP5376HZ		(0x90)

#define LSM330DLC_CTRL_REG1_A_LOWPOWER_ENABLE				(0x08)		/// Low power mode
#define LSM330DLC_CTRL_REG1_A_Z_ENABLE						(0x04)
#define LSM330DLC_CTRL_REG1_A_Y_ENABLE						(0x02)
#define LSM330DLC_CTRL_REG1_A_X_ENABLE						(0x01)

// ********************
// CTRL_REG2_A

// High pass filter mode
#define LSM330DLC_CTRL_REG2_A_HPM_NORMAL_RESET	(0x00)	/// Normal mode (reset reading HP_RESET_FILTER)
#define LSM330DLC_CTRL_REG2_A_HPM_REFERENCE		(0x40)	/// Reference signal for filtering
#define LSM330DLC_CTRL_REG2_A_HPM_NORMAL			(0x80)	/// Normal Mode
#define LSM330DLC_CTRL_REG2_A_HPM_INTAUTORESET	(0xC0)	/// Autoreset on interrupt event

/// @todo High-pass filter cut-off frequency


// Internal Filter
#define LSM330DLC_CTRL_REG2_A_FDS_BYPASS				(0x00)	/// Internal filter bypassed
#define LSM330DLC_CTRL_REG2_A_FDS_ENABLE				(0x08)	/// Data from internal filter

#define LSM330DLC_CTRL_REG2_A_HPCLICK_BYPASS			(0x00)	/// Internal filter bypassed for CLICK function
#define LSM330DLC_CTRL_REG2_A_HPCLICK_ENABLE			(0x04)	/// CLICK function from internal filter

#define LSM330DLC_CTRL_REG2_A_HPIS2_BYPASS			(0x00)	/// Internal filter bypassed for AOI function on interrupt 2
#define LSM330DLC_CTRL_REG2_A_HPIS2_ENABLE			(0x02)	/// High-pass filter enable for AOI function on interrupt 2

#define LSM330DLC_CTRL_REG2_A_HPIS1_BYPASS			(0x00)	/// Internal filter bypassed for AOI function on interrupt 1
#define LSM330DLC_CTRL_REG2_A_HPIS1_ENABLE			(0x01)	/// High-pass filter enable for AOI function on interrupt 1

// ********************
// CTRL_REG3_A

#define LSM330DLC_CTRL_REG3_A_I1_CLICK_ENABLE			(0x80)	/// Click interrupt on INT1_A
#define LSM330DLC_CTRL_REG3_A_I1_AOI1_ENABLE			(0x40)	/// AOI1 interrupt on INT1_A
													// Reserved bit. Must be 0
#define LSM330DLC_CTRL_REG3_A_I1_DRDY1_ENABLE			(0x01)	/// DRDY1 interrupt on INT1_A
#define LSM330DLC_CTRL_REG3_A_I1_DRDY2_ENABLE			(0x08)	/// DRDY2 interrupt on INT1_A
#define LSM330DLC_CTRL_REG3_A_I1_WTM_ENABLE			(0x04)	/// FIFO watermark interrupt on INT1_A
#define LSM330DLC_CTRL_REG3_A_I1_OVERRUN_ENABLE		(0x02)	/// FIFO overrun interrupt on INT1_A
													// Unused bit.

// *********************
// CTRL_REG4_A

													// Reserved bit. Must be 0
// Endianness
#define LSM330DLC_CTRL_REG4_A_BLE_LITTLEENDIAN		(0x00)		/// Little Endian Data 
#define LSM330DLC_CTRL_REG4_A_BLE_BIGENDIAN			(0x40)		/// Big Endian Data 

// Full-scale selection
#define LSM330DLC_CTRL_REG4_A_FS_2G					(0x00)		/// 2G (default)
#define LSM330DLC_CTRL_REG4_A_FS_4G					(0x10)		/// 4G
#define LSM330DLC_CTRL_REG4_A_FS_8G					(0x20)		/// 8G
#define LSM330DLC_CTRL_REG4_A_FS_16G				(0x30)		/// 16G

#define LSM330DLC_CTRL_REG4_A_HR_ENABLE				(0x08)		/// High-resolution mode enable.
													// Reserved bit. Must be 0.
													// Reserved bit. Must be 0.
#define LSM330DLC_CTRL_REG4_A_SPI_3WIRE				(0x01)	/// Enable SPI 3-wire mode


// *********************
// CTRL_REG5_A



#define LSM330DLC_CTRL_REG5_A_BOOT					(0x80)	/// Reboot memory content
#define LSM330DLC_CTRL_REG5_A_FIFO_ENABLE				(0x40)	/// FIFO enable
													// Unused bit.
													// Unused bit.
#define LSM330DLC_CTRL_REG5_A_LIR_INT1				(0x08)	/// Latch interrupt request on INT1_SRC register, with INT1_SRC register cleared by reading INT1_SRC itself.
#define LSM330DLC_CTRL_REG5_A_D4D_INT1				(0x04)	/// 4D enable: 4D detection is enabled on INT1 when 6D bit on INT1_CFG is set to 1
													// Reserved bit. Must be 0.
													// Reserved bit. Must be 0.



// *********************
// CTRL_REG6_A 

#define LSM330DLC_CTRL_REG6_A_I2_CLICK_ENABLE			(0x80)	/// Click interrupt on INT2_A.Default value 0.
#define LSM330DLC_CTRL_REG6_A_I2_INT1					(0x40)	/// Interrupt 1 function enabled on INT2_A.Default 0.
													// Reserved bit. Must be 0.
#define LSM330DLC_CTRL_REG6_A_BOOT_I2					(0x10)	/// Boot on INT2_A.
													// Reserved bit. Must be 0.
													// Unused bit.
#define LSM330DLC_CTRL_REG6_A_H_LACTIVE				(0x02)	/// 0: interrupt active high; 1: interrupt active low.

// *********************
// STATUS_REG_A
// STATUS_REG_G

#define LSM330DLC_STATUS_ZYXOR						(0x80)	/// X, Y, and Z axis data overrun.
#define LSM330DLC_STATUS_ZOR						(0x40)	/// Z axis data overrun.
#define LSM330DLC_STATUS_YOR						(0x20)	/// Y axis data overrun.
#define LSM330DLC_STATUS_XOR						(0x10)	/// X axis data overrun.
#define LSM330DLC_STATUS_ZYXDA						(0x08)	/// X, Y, and Z axis data available.
#define LSM330DLC_STATUS_ZDA						(0x04)	/// Z axis data available.
#define LSM330DLC_STATUS_YDA						(0x02)	/// Y axis data available.
#define LSM330DLC_STATUS_XDA						(0x01)	/// X axis data available.


// *********************
// FIFO_CTRL_REG_A


// FIFO Mode
#define LSM330DLC_FIFO_CTRL_REG_A_FIFO_BYPASS						(0x00)	/// Bypass mode.
#define LSM330DLC_FIFO_CTRL_REG_A_FIFO_FIFO							(0x40)	/// Fifo mode.
#define LSM330DLC_FIFO_CTRL_REG_A_FIFO_STREAM						(0x80)	/// Stream mode.
#define LSM330DLC_FIFO_CTRL_REG_A_FIFO_TRIGGER						(0xC0)	/// Trigger mode.

// Trigger selection

#define LSM330DLC_FIFO_CTRL_REG_A_TR_INT1_A							(0xC0)	/// Trigger event linked to trigger signal on INT1_A
#define LSM330DLC_FIFO_CTRL_REG_A_TR_INT1_A							(0xC0)	/// Trigger event linked to trigger signal on INT2_A
													// Reserved bit. Must be 0.
													// Reserved bit. Must be 0.
													// Reserved bit. Must be 0.
													// Reserved bit. Must be 0.
													// Reserved bit. Must be 0.


// *********************
// FIFO_SRC_REG_A

#define LSM330DLC_FIFO_SRC_REG_A_WTM								(0x80)	/// WTM bit is set high when FIFO content exceeds watermark level
#define LSM330DLC_FIFO_SRC_REG_A_OVRN_FIFO							(0x40)	/** OVRN bit is set high when FIFO buffer is full, this means that the FIFO buffer
																			 * contains 32 unread samples.At the following ODR a new sample set replaces the
																			 * oldest FIFO value.The OVRN bit is reset when the first sample set has been read
																			 */
#define LSM330DLC_FIFO_SRC_REG_A_EMPTY								(0x20)	/// EMPTY flag is set high when all FIFO samples have been read and FIFO is empty
#define LSM330DLC_FIFO_SRC_REG_A_FSS								(0x1F)	/** FSS[4:0] field always contains the current number of unread samples stored in the
																			 *	FIFO buffer.When FIFO is enabled, this value increases at ODR frequency until
																			 *	the buffer is full, whereas, it decreases every time that one sample set is retrieved
																			 *	from FIFO
																			 */



// *********************
// INT1_CFG_A

#define LSM330DLC_INT1_CFG_A_INTMODE_OR						(0x00)	/// OR combination of interrupt events
#define LSM330DLC_INT1_CFG_A_INTMODE_6DMOVEMENT				(0x40)	/// 6 direction movement recognition
#define LSM330DLC_INT1_CFG_A_INTMODE_AND						(0x80)	/// AND combination of interrupt events
#define LSM330DLC_INT1_CFG_A_INTMODE_6DPOSITION				(0xC0)	/// 6 direction position recognition

#define LSM330DLC_INT1_CFG_A_ZHIE_ZUPE							(0x20)	/// Enable interrupt generation on Z high event or on direction recognition.
#define LSM330DLC_INT1_CFG_A_ZLIE_ZDOWNE						(0x10)	/// Enable interrupt generation on Z low event or on direction recognition.
#define LSM330DLC_INT1_CFG_A_YHIE_YUPE							(0x08)	/// Enable interrupt generation on Y high event or on direction recognition.
#define LSM330DLC_INT1_CFG_A_YLIE_YDOWNE						(0x04)	/// Enable interrupt generation on Y low event or on direction recognition.
#define LSM330DLC_INT1_CFG_A_XHIE_XUPE							(0x02)	/// Enable interrupt generation on Z high event or on direction recognition.
#define LSM330DLC_INT1_CFG_A_XLIE_XDOWNE						(0x01)	/// Enable interrupt generation on Z low event or on direction recognition.



// *********************
// INT1_SRC_A (read only)

#define LSM330DLC_INT1_SRC_A_IA								(0x40)	/// Interrupt active, one or more interrupts have been generated.
#define LSM330DLC_INT1_SRC_A_ZH								(0x20)	/// Z high event has occurred.
#define LSM330DLC_INT1_SRC_A_ZL								(0x10)	/// Z low event has occurred.
#define LSM330DLC_INT1_SRC_A_YH								(0x08)	/// Y High event has occurred.
#define LSM330DLC_INT1_SRC_A_YL								(0x04)	/// Y Low event has occurred.
#define LSM330DLC_INT1_SRC_A_XH								(0x02)	/// X High event has occurred.
#define LSM330DLC_INT1_SRC_A_XL								(0x01)	/// X Low event has occurred.


// *********************
// INT1_THS_A

#define LSM330DLC_INT1_THS_A_MASK						(0x7F)	/// Interrupt 1 threshold mask.

// *********************
// INT1_DURATION_A

#define LSM330DLC_INT1_DURATION_A_MASK						(0x7F)	/// Interrupt 1 duration mask.



// *********************
// CLICK_CFG_A

															// Reserved bit. Must be 0.
															// Reserved bit. Must be 0.
#define LSM330DLC_CLICK_CFG_A_ZD									(0x20)	/// Enable interrupt double CLICK on Z axis.
#define LSM330DLC_CLICK_CFG_A_ZS									(0x10)	/// Enable interrupt single CLICK on Z axis.
#define LSM330DLC_CLICK_CFG_A_YD									(0x08)	/// Enable interrupt double CLICK on Y axis.
#define LSM330DLC_CLICK_CFG_A_YS									(0x04)	/// Enable interrupt single CLICK on Y axis.
#define LSM330DLC_CLICK_CFG_A_XD									(0x02)	/// Enable interrupt double CLICK on X axis.
#define LSM330DLC_CLICK_CFG_A_XS									(0x01)	/// Enable interrupt single CLICK on X axis.


// *********************
// CLICK_SRC_A

																// Unused bit.
#define LSM330DLC_CLICK_SRC_A_IA								(0x40)	/// Interrupt active, one or more interrupts have been generated)
#define LSM330DLC_CLICK_SRC_A_DCLICK 							(0x20)	/// Double CLICK - CLICK enable
#define LSM330DLC_CLICK_SRC_A_SCLICK 							(0x10)	/// Single CLICK - CLICK enable
#define LSM330DLC_CLICK_SRC_A_Sign								(0x08)	/// CLICK - CLICK Sign. 0 : positive detection, 1 : negative detection
#define LSM330DLC_CLICK_SRC_A_Z 								(0x04)	/// Z CLICK - CLICK detection (Z high event has occurred.)
#define LSM330DLC_CLICK_SRC_A_Y 								(0x02)	/// Y CLICK - CLICK detection (Y high event has occurred.)
#define LSM330DLC_CLICK_SRC_A_X 								(0x01)	/// X CLICK - CLICK detection (X high event has occurred.)

// *********************
// CLICK_THS_A

#define LSM330DLC_CLICK_THS_A_MASK							(0x7F)	/// Click 1 threshold mask.


// *********************
// TIME_LIMIT_A

#define LSM330DLC_TIME_LIMIT_A_MASK							(0x7F)	/// Click-click time limit.


// *********************
// ACT_THS

#define LSM330DLC_ACT_THS_MASK								(0x7F)	/// Sleep-to-wake. Return to sleep activation threshold (DUR = (Act_Dur + 1) * 8/ODR

// *********************
// WHO_AM_I_G

#define LSM330DLC_WHO_AM_I_G_VALUE								(0xD4)	/// Expected value of register.


// *********************
// CTRL_REG1_G

// Output data rate selection
// Bandwidth selection
#define LSM330DLC_CTRL_REG1_G_ODR_95HZ_BW12_5	   					(0x00)	/// ODR: 95HZ BW: 12.5
#define LSM330DLC_CTRL_REG1_G_ODR_95HZ_BW25							(0x10)	/// ODR: 95HZ BW: 25
#define LSM330DLC_CTRL_REG1_G_ODR_95HZ_BW25_B							(0x20)	   /// ODR: 95HZ BW: 25
#define LSM330DLC_CTRL_REG1_G_ODR_95HZ_BW25_C						(0x30)	   /// ODR: 95HZ BW: 25
#define LSM330DLC_CTRL_REG1_G_ODR_190HZ_BW12_5						(0x40)	   /// ODR: 190HZ BW: 12.5
#define LSM330DLC_CTRL_REG1_G_ODR_190HZ_BW25						(0x50)	   /// ODR: 190HZ BW: 25
#define LSM330DLC_CTRL_REG1_G_ODR_190HZ_BW50						(0x60)	   /// ODR: 190HZ BW: 50
#define LSM330DLC_CTRL_REG1_G_ODR_190HZ_BW70						(0x70)	   /// ODR: 190HZ BW: 70
#define LSM330DLC_CTRL_REG1_G_ODR_380HZ_BW20						(0x80)	   /// ODR: 380HZ BW: 20
#define LSM330DLC_CTRL_REG1_G_ODR_380HZ_BW25						(0x90)	   /// ODR: 380HZ BW: 25
#define LSM330DLC_CTRL_REG1_G_ODR_380HZ_BW50						(0xA0)	   /// ODR: 380HZ BW: 50
#define LSM330DLC_CTRL_REG1_G_ODR_380HZ_BW100						(0xB0)	   /// ODR: 380HZ BW: 100
#define LSM330DLC_CTRL_REG1_G_ODR_760HZ_BW30						(0xC0)	   /// ODR: 760HZ BW: 30
#define LSM330DLC_CTRL_REG1_G_ODR_760HZ_BW35						(0xD0)	   /// ODR: 760HZ BW: 35
#define LSM330DLC_CTRL_REG1_G_ODR_760HZ_BW50						(0xE0)	   /// ODR: 760HZ BW: 50
#define LSM330DLC_CTRL_REG1_G_ODR_760HZ_BW100						(0xF0)	   /// ODR: 760HZ BW: 100


#define LSM330DLC_CTRL_REG1_G_PD_ENABLE								(0x00)	   /// Power-down mode
#define LSM330DLC_CTRL_REG1_G_PD_NORMAL								(0x08)	   /// Normal mode (turn off all axes for sleep)

#define LSM330DLC_CTRL_REG1_G_Z_ENABLE								(0x04)	   /// Z axis enable (default 1)
#define LSM330DLC_CTRL_REG1_G_Y_ENABLE								(0x02)	   /// Y axis enable (default 1)
#define LSM330DLC_CTRL_REG1_G_X_ENABLE								(0x01)	   /// X axis enable (default 1)



// *********************
// CTRL_REG2_G

#define LSM330DLC_CTRL_REG2_G_EXTREN							(0x80)	/// Edge-sensitive trigger enable
#define LSM330DLC_CTRL_REG2_G_LVLEN								(0x40)	/// level-sensitve trigger enable

// High-pass filter mode configuration
#define LSM330DLC_CTRL_REG2_G_HPM_NORMAL_RESET				(0x00)	/// Normal mode (reset reading HP_RESET_FILTER)
#define LSM330DLC_CTRL_REG2_G_HPM_REFERENCE					(0x10)	/// Reference signal for filtering
#define LSM330DLC_CTRL_REG2_G_HPM_NORMAL					(0x20)	/// Normal Mode
#define LSM330DLC_CTRL_REG2_G_HPM_INTAUTORESET				(0x30)	/// Autoreset on interrupt event

// High-pass filter cut-off frequency configuration (see datasheet table 75)
#define LSM330DLC_CTRL_REG2_G_HPCF_0							(0x00)	/// (ODR=95Hz) 7.2Hz, (ODR=190Hz) 13.5Hz, (ODR=380Hz) 27Hz, (ODR=760Hz) 51.4Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_1							(0x01)	/// (ODR=95HZ) 3.5Hz, (ODR=190Hz) 7.2Hz, (ODR=380Hz) 13.5Hz, (ODR=760HZ) 27Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_2							(0x02)	/// (ODR=95HZ) 1.8Hz, (ODR=190Hz) 3.5Hz, (ODR=380Hz) 7.2Hz, (ODR=760HZ) 13.5Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_3							(0x03)	/// (ODR=95HZ) 0.9Hz, (ODR=190Hz) 1.8Hz, (ODR=380Hz) 3.5Hz, (ODR=760HZ) 7.2Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_4							(0x04)	/// (ODR=95HZ) 0.45Hz, (ODR=190Hz) 0.9Hz, (ODR=380Hz) 1.8Hz, (ODR=760HZ) 3.5Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_5							(0x05)	/// (ODR=95HZ) 0.18Hz, (ODR=190Hz) 0.45Hz, (ODR=380Hz) 0.9Hz, (ODR=760HZ) 1.8Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_6							(0x06)	/// (ODR=95HZ) 0.09Hz, (ODR=190Hz) 0.18Hz, (ODR=380Hz) 0.45Hz, (ODR=760HZ) 0.9Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_7							(0x07)	/// (ODR=95HZ) 0.045Hz, (ODR=190Hz) 0.09Hz, (ODR=380Hz) 0.18Hz, (ODR=760HZ) 0.45Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_8							(0x08)	/// (ODR=95HZ) 0.018Hz, (ODR=190Hz) 0.045Hz, (ODR=380Hz) 0.09Hz, (ODR=760HZ) 0.18Hz
#define LSM330DLC_CTRL_REG2_G_HPCF_9							(0x09)	/// (ODR=95HZ) 0.009Hz, (ODR=190Hz) 0.018Hz, (ODR=380Hz) 0.045Hz, (ODR=760HZ) 0.09Hz


// *********************
// CTRL_REG3_G

#define LSM330DLC_CTRL_REG3_G_I1_Int1				(0x80)	///   Interrupt enable on INT1_G pin.Default value 0. (0: Disable; 1: Enable)
#define LSM330DLC_CTRL_REG3_G_I1_Boot				(0x40)	///  Boot status available on INT1_G.Default value 0. (0: Disable; 1: Enable)
#define LSM330DLC_CTRL_REG3_G_H_Lactive				(0x20)	///  Interrupt active configuration on INT1_G.Default value 0. (0: High; 1:Low)
#define LSM330DLC_CTRL_REG3_G_PP_OD					(0x10)	///  Push - pull / Open drain.Default value : 0. (0: Push - pull; 1: Open drain)
#define LSM330DLC_CTRL_REG3_G_I2_DRDY				(0x08)	///  Date ready on DRDY_G / INT2_G.Default value 0. (0: Disable; 1: Enable)
#define LSM330DLC_CTRL_REG3_G_I2_WTM				(0x04)	///  FIFO watermark interrupt on DRDY_G / INT2_G.Default value : 0. (0: Disable; 1: Enable)
#define LSM330DLC_CTRL_REG3_G_I2_ORun				(0x02)	///  FIFO overrun interrupt on DRDY_G / INT2_G Default value : 0. (0: Disable; 1: Enable)
#define LSM330DLC_CTRL_REG3_G_I2_Empty				(0x01)	///  FIFO empty interrupt on DRDY_G / INT2_G.Default value : 0. (0: Disable; 1: Enable)



// *********************
// CTRL_REG4_G

#define LSM330DLC_CTRL_REG4_G_BDU				(0x80)	/// Block data update.  (0: continuous update; 1: output registers not updated until MSb and LSb reading)
#define LSM330DLC_CTRL_REG4_G_BLE 				(0x40)	/// Big / little endian data selection. (0: Data LSb @ lower address; 1: Data MSb @ lower address)
#define LSM330DLC_CTRL_REG4_G_FS_250DPS			(0x00)	/// FS Full scale selection.Default value : 00 (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
#define LSM330DLC_CTRL_REG4_G_FS_500DPS			(0x10)	/// FS Full scale selection.Default value : 00 (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
#define LSM330DLC_CTRL_REG4_G_FS_2000DPS		(0x20)	/// FS Full scale selection.Default value : 00 (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
#define LSM330DLC_CTRL_REG4_G_FS_2000DPS_B		(0x30)	/// FS Full scale selection.Default value : 00 (00: 250 dps; 01: 500 dps; 10: 2000 dps; 11: 2000 dps)
															// Unused bit.
															// Reserved bit. Must be 0.
															// Reserved bit. Must be 0.
#define LSM330DLC_CTRL_REG4_G_SIM 								(0x01)	/// 3 - wire SPI Serial interface read mode enable.Default value : 0 (0: 3 - wire Read mode disabled; 1: 3 - wire read enabled).



// *********************
// CTRL_REG5_G

#define LSM330DLC_CTRL_REG5_G_BOOT 								(0x80)	///  Reboot memory content.Default value : 0 (0: Normal mode; 1: reboot memory content)
#define LSM330DLC_CTRL_REG5_G_FIFO_EN 							(0x40)	///  FIFO enable.Default value : 0 (0: FIFO disable; 1: FIFO Enable)
#define LSM330DLC_CTRL_REG5_G_HPen 								(0x10)	///  High - pass filter Enable.Default value : 0 (0: HPF disabled; 1: HPF enabled, see Figure 20)

// See figure 20 in the datasheet for clarification.
#define LSM330DLC_CTRL_REG5_G_INT1_SEL_LPF1						(0x00)	///  Interrupt generated from LPF1.
#define LSM330DLC_CTRL_REG5_G_INT1_SEL_HPF						(0x04)	///  Interrupt generated from HPF.
#define LSM330DLC_CTRL_REG5_G_INT1_SEL_LPF2						(0x08)	///  Interrupt generated from LPF2.
#define LSM330DLC_CTRL_REG5_G_INT1_SEL_LPF2_B					(0x0C)	///  Interrupt generated from LPF2.

// See figure 20 in the datasheet for clarification.
#define LSM330DLC_CTRL_REG5_G_OUT_SEL_LPF1						(0x00)	///  Output generated from LPF1.
#define LSM330DLC_CTRL_REG5_G_OUT_SEL_HPF						(0x04)	///  Output generated from HPF.
#define LSM330DLC_CTRL_REG5_G_OUT_SEL_LPF2						(0x08)	///  Output generated from LPF2.
#define LSM330DLC_CTRL_REG5_G_OUT_SEL_LPF2_B					(0x0C)	///  Output generated from LPF2.


// *********************
// FIFO_CTRL_REG_G

#define LSM330DLC_FIFO_CTRL_REG_G_BYPASS						(0x00)	/// Bypass mode
#define LSM330DLC_FIFO_CTRL_REG_G_FIFO 							(0x20)	/// FIFO mode
#define LSM330DLC_FIFO_CTRL_REG_G_STREAM						(0x40)	/// Stream mode
#define LSM330DLC_FIFO_CTRL_REG_G_STREAMTOFIFO					(0x60)	/// Stream - to - FIFO mode
#define LSM330DLC_FIFO_CTRL_REG_G_BYPASSTOSTREAM				(0x80)	/// Bypass - to - stream mode

#define LSM330DLC_FIFO_CTRL_REG_G_WTM_MASK						(0x1F)	/// Bypass - to - stream mode



// *********************
// INT1_CFG_G

#define LSM330DLC_FIFO_INT1_CFG_G_ANDOR								(0x80)	/// AND / OR combination of interrupt events.Default value : 0 (0: OR combination of interrupt events 1 : AND combination of interrupt events
#define LSM330DLC_FIFO_INT1_CFG_G_LIR								(0x40)	/// Latch Interrupt request.Default value : 0 (0: interrupt request not latched; 1: interrupt request latched) Cleared by reading INT1_SRC_G reg.
#define LSM330DLC_FIFO_INT1_CFG_G_ZHIE								(0x20)	/// Enable interrupt generation on Z high event.Default value : 0 (0: disable interrupt request; 1: enable interrupt request on measured value higher than preset threshold)
#define LSM330DLC_FIFO_INT1_CFG_G_ZLIE								(0x10)	/// Enable interrupt generation on Z low event.Default value : 0 (0: disable interrupt request; 1: enable interrupt request on measured value lower than preset threshold)
#define LSM330DLC_FIFO_INT1_CFG_G_YHIE								(0x08)	/// Enable interrupt generation on Y high event.Default value : 0 (0: disable interrupt request; 1: enable interrupt request on measured value higher than preset threshold)
#define LSM330DLC_FIFO_INT1_CFG_G_YLIE								(0x04)	/// Enable interrupt generation on Y low event.Default value : 0 (0: disable interrupt request; 1: enable interrupt request on measured value lower than preset threshold)
#define LSM330DLC_FIFO_INT1_CFG_G_XHIE								(0x02)	/// Enable interrupt generation on X high event.Default value : 0 (0: disable interrupt request; 1: enable interrupt request on measured value higher than preset threshold)
#define LSM330DLC_FIFO_INT1_CFG_G_XLIE								(0x01)	/// Enable interrupt generation on X low event.Default value : 0 (0: disable interrupt request; 1: enable interrupt request on measured value lower than preset threshold)



// *********************
// INT1_SRC_G


#define LSM330DLC_FIFO_INT1_SRC_G_IA // Interrupt active. Default value : 0 (0: no interrupt has been generated; 1: one or more interrupts have been generated)
#define LSM330DLC_FIFO_INT1_SRC_G_ZH // Z high. Default value : 0 (0: no interrupt, 1 : Z High event has occurred)
#define LSM330DLC_FIFO_INT1_SRC_G_ZL // Z low. Default value : 0 (0: no interrupt; 1: Z Low event has occurred)
#define LSM330DLC_FIFO_INT1_SRC_G_YH // Y high. Default value : 0 (0: no interrupt, 1 : Y High event has occurred)
#define LSM330DLC_FIFO_INT1_SRC_G_YL // Y low. Default value : 0 (0: no interrupt, 1 : Y Low event has occurred)
#define LSM330DLC_FIFO_INT1_SRC_G_XH //	X high. Default value : 0 (0: no interrupt, 1 : X High event has occurred)
#define LSM330DLC_FIFO_INT1_SRC_G_XL //	X low. Default value : 0 (0: no interrupt, 1 : X Low event has occurred)



// *********************
// INT1_DURATION_G

#define LSM330DLC_INT1_DURATION_G_WAIT								(0x80)	/// Wait enable.
























