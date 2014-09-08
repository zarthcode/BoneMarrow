#include "IMU.h"
#include "Semihosting.h"


/**
  * @brief Tx Transfer completed callbacks
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{

	// This method is reserved for the interrupt-based SPI1_TX(IMU_P6) workaround.
	if (hspi == &hspi1)
		IMU_SPI1_Handle_IT();
	else
	{
		printf_semi("Improper call of HAL_SPI_TxCpltCallback()\n");
	}
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
	IMU_PortType port = IMU_GetPortFromSPIHandle(hspi);

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
	IMU_PortType port = IMU_GetPortFromSPIHandle(hspi);

	// Allow the spi event to be handled immediately.
	IMU_HandleSPIEvent(port);

}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{

	printf_semi("HAL_SPI_ErrorCallback() - Damn, you!\n");
}
// void HAL_SPI_TxHalfCpltCallback(SPI_HandleTypeDef *hspi);
// void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef *hspi);
// void HAL_SPI_TxRxHalfCpltCallback(SPI_HandleTypeDef *hspi);
