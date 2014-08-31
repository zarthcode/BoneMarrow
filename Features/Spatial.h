/** @file Spatial.h
 *
 * Spatial structures and operations
 *
 */


typedef struct 
{
	float x;
	float y;
	float z;
	float w;

} Quaternion;


// Vector3
typedef struct
{
	float x;
	float y;
	float z;

} Vector3;

/// 16-bit (twos compliment) Vector3
typedef struct 
{
	uint8_t pad;		/// Padding byte, for alignment purposes.
	uint8_t txbyte;		/// Used to hold the spi address byte.
	struct 
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};
} IVector3;

/// Unscaled Acceleration, Gyroscope, and Magnetometer Data
typedef struct 
{

	IVector3 accelerometer;
	IVector3 gyroscope;
	IVector3 magnetometer;
	uint32_t tickstamp;			/// @bug This is a problem every 47.1 days

} IMU_RAW;


/// Acceleration, Gyroscope, and Magnetometer Data
typedef struct 
{

	Vector3 accelerometer;
	Vector3 gyroscope;
	Vector3 magnetometer;
	uint32_t tickstamp;			/// @bug This is a problem every 47.1 days

} IMU_SCALED;



/// Converts an IVector3 into a Vector3
/// @todo Return Vector3, and evaluate optimization/speed result.
void ToVector3(IVector3* raw, Vector3* out, float fullscale);

