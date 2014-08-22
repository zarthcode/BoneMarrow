// IMU SPI implementation

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "IMU.h"
#include "Semihosting.h"
#include <string.h>
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
					uint8_t addressbyte = LSM330DLC_FormatAddress(true, false, LSM330DLC_REG_WHO_AM_I_G);

					// Send a byte...
					if (HAL_SPI_Transmit(IMUDevice[imuport].hspi, &addressbyte, 1, SPI_TIMEOUT) != HAL_OK)
					{
						// No! Bad! Bad STM32! 
						printf_semi("SPI IMU Test (transmit) failed.\n");
				
					}
					// Read a byte...
					uint8_t responsebyte = 0;

					if (HAL_SPI_Receive(IMUDevice[imuport].hspi, &responsebyte, 1, SPI_TIMEOUT) != HAL_OK)
					{
						// No! Bad! Bad IMU! 
						printf_semi("SPI IMU Test (receive) failed.\n");
					}

					// Reset CS
					SelectIMUSubDevice(imuport, IMU_SUBDEV_NONE);

					// ...Result should equal 0b11010100
					if (responsebyte == LSM330DLC_WHO_AM_I_G_VALUE)
					{
						// Ready to go!
						printf_semi("SPI IMU Test successful. (Received 0x%02x)\n", responsebyte);
						return true;
						
					} 
					else
					{
						// Ready to go!
						printf_semi("SPI IMU Test unsuccessful (received 0x%02x).\n", responsebyte);
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
			
			printf_semi("Cannot test implemented device IMU_Device (%d)", IMUDevice[imuport].DeviceName);
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
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (IMU_SUBDEV_ACC == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
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
		// SPI6
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

			// Configure Interrupts
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


			// Test IMU connectivity

			// Configure IMU settings.

			// Configure DMA

			break;
		case IMU_P1:
			printf_semi("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			break;
		case IMU_P2:
			printf_semi("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			break;
		case IMU_P3:
			printf_semi("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			break;
		case IMU_P4:
			printf_semi("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			break;
		case IMU_P5:
			printf_semi("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			break;
		case IMU_P6:
			printf_semi("SetupIMU() encountered an unimplemented IMU_PortType (%d).\n", i);
			break;
		default:
			// This is serious.
			#ifdef DEBUG
			// UsageFault_Handler();
			__BKPT(0);
			printf_semi("SetupIMU() encountered an unfamiliar IMU_PortType.\n");
			#endif
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
			printf_semi("SPI not yet initialized or disabled.\n");
			break;
		HAL_SPI_STATE_READY:
			printf_semi("SPI initialized and ready for use.\n");
			break;
		HAL_SPI_STATE_BUSY:
			printf_semi("SPI process is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_TX:
			printf_semi("SPI transmission is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_RX:
			printf_semi("SPI reception is ongoing.\n");
			break;
		HAL_SPI_STATE_BUSY_TX_RX:
			printf_semi("SPI transmission/reception is ongoing.\n");
			break;
		HAL_SPI_STATE_ERROR:
			printf_semi("SPI error state.\n");
			break;
		default:
			printf_semi("SPI unknown state.\n");
			break;
	}
	
}

void IMU_StartDMATransfer(IMU_PortType finger, IMU_SubDeviceType component)
{
	// See ST application notes:
	//		AN3109 - FIFO Emulation w/DMA Controller w/Timeout interrupts (http://goo.gl/utJwbF)
	//		AN4031 - Using the STM32F2 and STM32F4 DMA controller (http://goo.gl/TmhTv9)

	// 

	// Configure DMA Channel

}

void ConfigureIMU(IMU_PortType finger)
{
	switch (IMUDevice[finger].DeviceName)
	{

	case IMU_DEVICE_LSM330DLC:
		{
			SelectIMUSubDevice(finger, IMU_SUBDEV_ACC);

			uint8_t configurationPacket[7];
		// Accelerometer setup.
			// Configure address packet
			configurationPacket[0] = LSM330DLC_FormatAddress(false, true, LSM330DLC_REG_CTRL_REG1_A);

			// Reset IMU
				// Power off.
			configurationPacket[1] = LSM330DLC_CTRL_REG1_A_ODR_POWERDOWN;
			
			// Setup internal filter
			configurationPacket[2] = LSM330DLC_CTRL_REG2_A_HPM_NORMAL_RESET |
										LSM330DLC_CTRL_REG2_A_FDS_ENABLE;	

			// Interrupt configuration
			configurationPacket[3] = LSM330DLC_CTRL_REG3_A_I1_DRDY1_ENABLE |
									LSM330DLC_CTRL_REG3_A_I1_DRDY2_ENABLE;
			// Full scale, endianness, resolution 
			configurationPacket[4] = LSM330DLC_CTRL_REG4_A_BLE_LITTLEENDIAN |
				LSM330DLC_CTRL_REG4_A_FS_2G;
			
			// FIFO reset, FIFO settings, Interrupt latch, 4D detection
			configurationPacket[5] = LSM330DLC_CTRL_REG5_A_BOOT;

			// Active high interrupt, no INT2_A, for now.
			configurationPacket[6] = 0;
									

			// Send data
			HAL_StatusTypeDef result = HAL_SPI_Transmit(IMUDevice[finger].hspi, configurationPacket, sizeof(configurationPacket), 200);
			if(result != HAL_OK)
			{
#ifdef DEBUG
				printf_semi("IMU acc setup failed (%d)\n", result);

				SelectIMUSubDevice(finger, IMU_SUBDEV_NONE);
				__BKPT(0);
				return;
#endif // DEBUG

			}

			
			SelectIMUSubDevice(finger, IMU_SUBDEV_GYRO);
			
			// clear
			memset(configurationPacket, 0, sizeof(configurationPacket));


		// Gyroscope

			// Address packet
			configurationPacket[0] = LSM330DLC_FormatAddress(false, true, LSM330DLC_REG_CTRL_REG1_G);

			// CTRL_REG1_G - power everything off.
			configurationPacket[1] = LSM330DLC_CTRL_REG1_G_PD_ENABLE;


			// CTRL_REG2_G - Reset filter, default HP cutoff frequency.
			configurationPacket[2] = LSM330DLC_CTRL_REG2_G_HPM_NORMAL_RESET
									| LSM330DLC_CTRL_REG2_G_HPCF_0;

			// CTRL_REG3_G - Interrupt settings
			configurationPacket[3] = LSM330DLC_CTRL_REG3_G_H_Lactive;

			// CTRL_REG4_G - Endianness, Full scale selection
			configurationPacket[4] = LSM330DLC_CTRL_REG4_G_FS_250DPS;

			// CTRL_REG5_G - FIFO, high pass filter, Output selection, Interrupt selection
			configurationPacket[5] = LSM330DLC_CTRL_REG5_G_HPen
									| LSM330DLC_CTRL_REG5_G_OUT_SEL_HPF;

			// Send
			result = HAL_SPI_Transmit(IMUDevice[finger].hspi, configurationPacket, sizeof(configurationPacket) - 1, 200);
			if(result != HAL_OK)
			{
				printf_semi("IMU gyro setup failed (%d)\n", result);
				return;

			}

			SelectIMUSubDevice(finger, IMU_SUBDEV_NONE);


			// ? Power on
			// ODR / Power On
				// Accelerometer ODR (1Hz, 10Hz, 25Hz, 50Hz, 100Hz, 200Hz, 400Hz, 1.62kHz, 1.344kHz)
				// Gyroscope ODR (95Hz, 190Hz, 380Hz, 760Hz)
		}
		break;

	/// @todo Support for 9-axis devices (LSM9DS0)
	/// @todo Support for external-brains (LSM6DB0)



		break;
	}

	// Setup complete.

}
