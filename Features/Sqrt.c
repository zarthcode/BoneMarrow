#include "Sqrt.h"


IMU_SQRTMethodType _imu_sqrt_method = IMU_SQRT_FAST2ND;

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float OPTIMIZED_invSqrt(float x)
{

	switch (_imu_sqrt_method)
	{

		case IMU_SQRT_FAST:
		case IMU_SQRT_FAST2ND:
		{
			float halfx = 0.5f * x;
			float y = x;
			long i = *(long*)&y;
			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;
			y = y * (1.5f - (halfx * y * y));

			if (_imu_sqrt_method == IMU_SQRT_FAST2ND)
				y = y * (1.5f - (halfx * y * y));
		
			return y;

		}
			break;


		case IMU_SQRT_FASTACC:
		{

			unsigned int i = 0x5F1F1412 - (*(unsigned int*)&x >> 1);
			float tmp = *(float*)&i;
			float y = tmp * (1.69000231f - 0.714158168f * x * tmp * tmp);

			return y;

		}
		break;
		
		/// @TODO Implement other algoritms
		/// @TODO Implement ARM CMSIS fast sqrt
		/// @TODO Write test/eval suite
		/// @TODO Add selection configuration from emulated FLASH memory

		default:
#ifdef DEBUG
			printf_semi("Unimplemented Sqrt Function requested (%d)\n", _imu_sqrt_method);
			__BKPT(0);
#endif
			break;
	}

	return 0.0f;
}

