#include "arm_math.h"
#include "Semihosting.h"
/// \file Selectable Square-root
typedef enum { IMU_SQRT_FAST, IMU_SQRT_FAST2ND, IMU_SQRT_FASTACC, IMU_SQRT_SLOW } IMU_SQRTMethodType;

extern IMU_SQRTMethodType _imu_sqrt_method;



/**
 * @brief Calculates the inverse sqrt using the currently selected method.
 * @returns float answer/inverse-sqrt approximation.
 * @param float x
 */
float OPTIMIZED_invSqrt(float x);

/* Intended to replace stdlib sqrt.
__INLINE float sqrt(float x)
{
	float ret;
	int result = arm_sqrt_f32(x, &ret);
	
#ifdef DEBUG
	if (result != ARM_MATH_SUCCESS)
	{
		printf_semi("BAD - sqrt argument error.\n");
		__BKPT(0);
	}
#endif // DEBUG

	return ret;
};
*/