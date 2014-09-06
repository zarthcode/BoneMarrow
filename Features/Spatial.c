/// @file Spatial orientation methods implmentation

#include "Spatial.h"
#include "Semihosting.h"

IMU_SCALED_FramebufferType SPATIAL_IMUFrameBuffer[SPATIAL_IMUFRAMEBUFFER_SIZE];

uint8_t SPATIAL_IMUFrameBuffer_ReadIndex = 0;
uint8_t SPATIAL_IMUFramwBuffer_WriteIndex = 0;

void SPATIAL_RAWToVector3(IVector3* raw, Vector3* out, float fullscale)
{

	// Lets hope the optimizer does the obvious w/these floating operations...

	// Do the rescale
	out->x = (raw->x >= 0) ? (float)raw->x * ( fullscale / (float)INT16_MAX ) : (float)raw->x * ( fullscale / (float)INT16_MIN )  ;
	out->y = (raw->y >= 0) ? (float)raw->y * ( fullscale / (float)INT16_MAX ) : (float)raw->y * ( fullscale / (float)INT16_MIN )  ;
	out->z = (raw->z >= 0) ? (float)raw->z * ( fullscale / (float)INT16_MAX ) : (float)raw->z * ( fullscale / (float)INT16_MIN )  ;

}

void SPATIAL_RAWToRadiansPerSec(Vector3* dps_gyro)
{
	const float radian = 0.0174532925f; // Yeah, it's like that.
	
	dps_gyro->x *= radian;
	dps_gyro->y *= radian;
	dps_gyro->z *= radian;
}

bool DIAG_SPATIAL_BufferStatistics(void)
{
	// Spit-out a bunch of useful development info.
	printf_semi("DIAG_SPATIAL_BufferStatistics()\n");

	// Structure sizes
	printf_semi("\tIMU_SCALED structure is %d bytes.\n", sizeof(IMU_SCALED));
	printf_semi("\tIMU_RAW structure is %d bytes.\n", sizeof(IMU_RAW));
	printf_semi("\tQuaternion structure is %d bytes.\n", sizeof(Quaternion));
	printf_semi("\tVector3 structure is %d bytes.\n", sizeof(Vector3));
	printf_semi("\tIVector3 structure is %d bytes.\n", sizeof(IVector3));
	printf_semi("\tSPATIAL_IMUFramebuffer[%d] array is %d bytes.\n", SPATIAL_IMUFRAMEBUFFER_SIZE, SPATIAL_IMUFRAMEBUFFER_SIZE * sizeof(IMU_SCALED));

	/// @todo bool DIAG_SPATIAL_BufferStatistics(void) Some benchmarking would be wonderful...

	
	// Don't change this until the test actually validates expectations.
	return false;
}
