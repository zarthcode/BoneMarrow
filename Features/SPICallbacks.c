#include "IMU.h"


/**
  * @brief Tx Transfer completed callbacks
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{

	// This would be the time to kick off a pending RX, if there is one.

	/// @note After T_hold - Release \CS line.
	/// @bug T_hold needs to be verified.
	SelectIMUSubDevice(GetIMU(hspi), IMU_SUBDEV_NONE);

}

/**
  * @brief Rx Transfer completed callbacks
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	/// @note After T_hold - Release \CS line.
	/// @bug T_hold needs to be verified.
	SelectIMUSubDevice(GetIMU(hspi), IMU_SUBDEV_NONE);

}

/**
  * @brief Tx and Rx Transfer completed callbacks
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

	// Handle SPI event

	IMU_PortType imu = GetIMU(hspi);

	// Place tick timestamp
//	IMUDevice->current_frame->tickstamp = HAL_GetTick();

	/// @note After T_hold - Release \CS line.
	/// @bug T_hold needs to be verified.
	SelectIMUSubDevice(imu, IMU_SUBDEV_NONE);

	// Start AHRS calculation.

}
