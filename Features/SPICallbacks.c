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
	IMU_SelectSubDevice(IMU_GetFromSPIHandle(hspi), IMU_SUBDEV_NONE);

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

	// Determine the port and subdevice.
	IMU_PortType port = IMU_GetFromSPIHandle(hspi);

	// Allow the spi event to be handled immediately.
	IMU_HandleSPIEvent(port);

}

/**
  * @brief Tx and Rx Transfer completed callbacks
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

	// Determine the port and subdevice.
	IMU_PortType port = IMU_GetFromSPIHandle(hspi);

	// Allow the spi event to be handled immediately.
	IMU_HandleSPIEvent(port);

}
