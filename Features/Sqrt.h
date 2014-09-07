/// \file Selectable Square-root
typedef enum { IMU_SQRT_FAST, IMU_SQRT_FAST2ND, IMU_SQRT_FASTACC, IMU_SQRT_SLOW } IMU_SQRTMethodType;

extern IMU_SQRTMethodType _imu_sqrt_method;



/**
 * @brief Calculates the inverse sqrt using the currently selected method.
 * @returns float answer/inverse-sqrt approximation.
 * @param float x
 */
float invSqrt(float x);
