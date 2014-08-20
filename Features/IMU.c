// IMU SPI implementation

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "IMU.h"
#include <stdio.h>
#include <string.h>
/// IMU Map
IMU_MappingStruct IMUDevice[IMU_LAST];

/// Debugging utility function.
void DBG_SPICheckState(HAL_SPI_StateTypeDef state);



/// Determines if the SPI port can communicate with the IMU.
bool IMUTest(IMU_PortType imuport)
{
	/// @bug - we are currently assuming Alpha-0 IMUs (LSM330DLC)
	switch (IMUDevice[imuport].DeviceName)
	{
		
		case IMU_DEVICE_LSM330DLC:

			{
				// Is SPI ready?
				HAL_SPI_StateTypeDef spistateresult = HAL_SPI_GetState(IMUDevice[imuport].hspi);

				if (spistateresult == HAL_SPI_STATE_READY)
				{

					// Select the gyroscope
					SelectIMUSubDevice(imuport, IMU_SUBDEV_GYRO);

					// Access WHO_AM_I_G register at address 0F.
					uint8_t addressbyte = LSM330DLC_FormatAddress(true, false, LSM330DLC__WHO_AM_I_G);

					// Send a byte...
					if (HAL_SPI_Transmit(IMUDevice[imuport].hspi, &addressbyte, 1, SPI_TIMEOUT) != HAL_OK)
					{
						// No! Bad! Bad STM32! 
						#ifdef DEBUG
						printf("SPI IMU Test (transmit) failed.\n");
						#endif
				
					}
					// Read a byte...
					uint8_t responsebyte = 0;

					if (HAL_SPI_Receive(IMUDevice[imuport].hspi, &responsebyte, 1, SPI_TIMEOUT) != HAL_OK)
					{
						// No! Bad! Bad IMU! 
						#ifdef DEBUG
						printf("SPI IMU Test (receive) failed.\n");
						#endif
					}

					// ...Result should equal 0b11010100
					if (responsebyte == 0xD4)
					{
						// Ready to go!
						#ifdef DEBUG
						printf("SPI IMU Test successful. (Received 0x%02x)\n", responsebyte);
						#endif
						return true;
						
					} 
					else
					{
						// Ready to go!
						#ifdef DEBUG
						printf("SPI IMU Test unsuccessful (received 0x%02x).\n", responsebyte);
						#endif
					}
				}
				else
				{
				#ifdef DEBUG
					DBG_SPICheckState(spistateresult);
				#endif
				}

			}
			break;

		default:
			
			#ifdef DEBUG
			printf("Cannot test implemented device IMU_Device (%d)", IMUDevice[imuport].DeviceName);
			#endif
			break;
		
	}
	return false;
}

void SelectIMUSubDevice(IMU_PortType finger, IMU_SubDeviceType component)
{
	/// @bug It may be better to specifically deactivate all components, first.

	// Select the requested component
	for (int i = 0; i < IMU_SUBDEV_NONE; i++)
	{
		// Disable
		
		if (IMUDevice[finger].CSMappings[i].port != NULL)		// Paranoia, until it isn't.
		{
			HAL_GPIO_WritePin(IMUDevice[finger].CSMappings[i].port, IMUDevice[finger].CSMappings[i].pin, (i == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
		}
	}

	// Mirror onto PB4/PB5 (debug)
#ifdef DEBUG
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (IMU_SUBDEV_ACC == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (IMU_SUBDEV_GYRO == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
#endif

}

void SetupIMU(void)
{
	
	/// @bug - I don't know if this is necessary on arm-gcc....
	memset(IMUDevice, 0, sizeof(IMUDevice));
	
	/// @bug This needs to be done via auto-detection.
	for (int i = 0; i < IMU_LAST; i++)
	{
		switch (i)
		{
		case IMU_ONBOARD:				/// LSM330DLC (EVP4)
			IMUDevice[i].DeviceName = IMU_DEVICE_LSM330DLC;
			IMUDevice[i].hasMagnetometer = false;
			IMUDevice[i].IMUType = IMU_DEV_6AXIS;		/// @todo This needs auto-detection.
			IMUDevice[i].hspi = &hspi6;
			// CS2 (accelerometer)
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].port = GPIOB;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].pin = GPIO_PIN_8;
			IMUDevice[i].CSMappings[IMU_SUBDEV_ACC].type = IMU_SUBDEV_ACC;
			// CS3 (gyroscope)
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].port = GPIOB;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].pin = GPIO_PIN_9;
			IMUDevice[i].CSMappings[IMU_SUBDEV_GYRO].type = IMU_SUBDEV_GYRO;

			SelectIMUSubDevice(i, IMU_SUBDEV_NONE);		// Deselect the device.

			// INT2
			IMUDevice[i].INTMappings[0].port = GPIOG;
			IMUDevice[i].INTMappings[0].pin = GPIO_PIN_10;
			IMUDevice[i].INTMappings[0].type = IMU_SUBDEV_ACC;
			// INT3
			IMUDevice[i].INTMappings[1].port = GPIOG;
			IMUDevice[i].INTMappings[1].pin = GPIO_PIN_11;
			IMUDevice[i].INTMappings[1].type = IMU_SUBDEV_ACC;
			// INT4
			IMUDevice[i].INTMappings[2].port = GPIOE;
			IMUDevice[i].INTMappings[2].pin = GPIO_PIN_0;
			IMUDevice[i].INTMappings[2].type = IMU_SUBDEV_GYRO;
			// INT5
			IMUDevice[i].INTMappings[3].port = GPIOE;
			IMUDevice[i].INTMappings[3].pin = GPIO_PIN_1;
			IMUDevice[i].INTMappings[3].type = IMU_SUBDEV_GYRO;

			// Configure Interrupts

			// Test IMU connectivity

			// Configure IMU settings.
				// Filter
				// FIFO/Bypass
				// Stream mode

			break;
		case IMU_P1:
			#ifdef DEBUG
			printf("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			#endif
			break;
		case IMU_P2:
			#ifdef DEBUG
			printf("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			#endif
			break;
		case IMU_P3:
			#ifdef DEBUG
			printf("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			#endif
			break;
		case IMU_P4:
			#ifdef DEBUG
			printf("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			#endif
			break;
		case IMU_P5:
			#ifdef DEBUG
			printf("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			#endif
			break;
		case IMU_P6:
			#ifdef DEBUG
			printf("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			#endif
			break;
		default:
			// This is serious.
			#ifdef DEBUG
			printf("SetupIMU() encountered an unfamiliar IMU_PortType.\n");
			#endif
			// UsageFault_Handler();
			break;

		}
	}

}

void IMU_ConfigureEXTI(IMU_PinMappingType* pinmap)
{

	

}

void DBG_SPICheckState(HAL_SPI_StateTypeDef state)
{
	switch (state)
	{
		HAL_SPI_STATE_RESET:
			printf("SPI not yet initialized or disabled.\n");
			break;
		HAL_SPI_STATE_READY:
			printf("SPI initialized and ready for use.\n");
			break;
		HAL_SPI_STATE_BUSY:
			printf("SPI process is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_TX:
			printf("SPI transmission is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_RX:
			printf("SPI reception is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_TX_RX:
			printf("SPI transmission/reception is ongoing.\n");
			break;
		HAL_SPI_STATE_ERROR:
			printf("SPI error state.\n");
			break;
		default:
			printf("SPI unknown state.\n");
			break;
	}
	
}