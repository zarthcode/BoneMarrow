/** @file Spatial.h
 *
 * Spatial structures and operations
 *
 */

/// Data-type for 3D quaternions
typedef struct 
{
	float x;	/// X component
	float y;	/// Y component
	float z;	/// Z component
	float w;	/// W component

} Quaternion;


/// Container for scaled/converted data.
typedef struct
{
	float x;	/// X-axis component
	float y;	/// Y-axis component
	float z;	/// Z-axis component

} Vector3;

/// 16-bit (twos compliment) container for raw data from an imu.
typedef struct
#ifndef DOXYGEN
__attribute__((packed)) 
#endif // !DOXYGEN
{
	/// @note defining the preprocessor macro IMU_RAW_PADDING will add a uint8_t to the beginning of the structure.

	/// @bug An alternate structure will be needed to support burst reads from future imu devices.
#ifdef IMU_RAW_PADDING
	uint8_t pad;		/// Optional Padding byte, for alignment purposes.
#endif
	uint8_t txbyte;		/// Used to hold the spi address byte.
	struct 
	{
		int16_t x;		/// X-axis component
		int16_t y;		/// Y-axis component
		int16_t z;		/// Z-axis component
	};
} IVector3;

/// Unscaled Acceleration, Gyroscope, and Magnetometer Data
/// @todo Dynamically align raw data reads using pointers and a raw memory block.
typedef struct 
{

	IVector3 accelerometer;		/// Stores accelerometer data direct from the imu.
	IVector3 gyroscope;			/// Stores gyroscope data direct from the imu.
	IVector3 magnetometer;		/// Stores magnetometer data, if available.
	uint32_t tickstamp;			/// Stores the timestamp for this dataset
								/// @bug This timestamp is a problem every 47.1 days
} IMU_RAW;


/// Acceleration, Gyroscope, and Magnetometer Data
typedef struct 
{

	Vector3 accelerometer;		/// Stores scaled accelerometer data.  (g)
	Vector3 gyroscope;			/// stores scaled gyroscope data	(radians/sec)
	Vector3 magnetometer;		/// Stores magnetometer data
								/// @bug mag support is unimplemented.
	uint32_t tickstamp;			/// @bug This timestamp is a problem every 47.1 days

} IMU_SCALED;

#define SPATIAL_IMUFRAMEBUFFER_SIZE 8

/// Scaled IMU data.
extern IMU_SCALED SPATIAL_IMUFrameBuffer;


/// @todo Return Vector3, and evaluate optimization/speed result.
/**
 * @brief Converts an IVector3 into a scaled Vector3
 * @param[in] raw Unscaled imu input.
 * @param[out] out Rescaled imu data
 * @param[in] fullscale Current fullscale setting of the imu (see datasheet for units and values)
 */
void SPATIAL_RAWToVector3(IVector3* raw, Vector3* out, float fullscale);

/// @todo Consider converting from IVector3 in one step.
/**
 * @brief Converts degrees/s to rad/sec.
 * @param[in,out] dps_gyro Gyroscope input, in degrees per second.  To be converted, in-place.
 */
void SPATIAL_RAWToRadiansPerSec(Vector3* dps_gyro);

/// Diagnostic Information
bool DIAG_SPATIAL_BufferStatistics(void);
