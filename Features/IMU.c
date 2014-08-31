// IMU SPI implementation

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "IMU.h"
#include "Semihosting.h"
#include <string.h>
#include <string.h>

/// IMU Map
IMU_MappingStruct IMUDevice[IMU_LAST];

/// IMU data buffer
IMU_RAWFrame IMU_RAWFramebuffer[IMU_FRAMEBUFFER_SIZE];		/// @todo Implement FIFO 

/// Number of IMU framebuffers pending
uint8_t volatile IMU_RAWFramebuffersPending = 0;
uint8_t volatile IMU_RAWFramebuffer_ReadIndex = 0;
uint8_t volatile IMU_RAWFramebuffer_WriteIndex = 0;

/// Transfer state
IMU_TransferStateType IMU_TransferState;




/// Debugging utility function.
void DBG_SPICheckState(HAL_SPI_StateTypeDef state);


/// Test IVector3 alignment
void IMU_CheckAlignment(void)
{
	// Quick analysis of IVector3 alignment
	IVector3 testbyte;

	testbyte.txbyte = 0xDE;
	testbyte.x = 0;
	testbyte.y = INT16_MAX;
	testbyte.z = 0;

	// printout data
	printf_semi("IMU_CheckAlignment() - Test byte\n");
	printf_semi("testbyte.txbyte = %d [0x%02x] \t\t[address = %p]\n", testbyte.txbyte, testbyte.txbyte, &testbyte.txbyte);
	printf_semi("testbyte.x = %d [0x%04x] \t[address = %p]\n", testbyte.x, testbyte.x, &testbyte.x);
	printf_semi("testbyte.y = %d [0x%04x] \t[address = %p]\n", testbyte.y, testbyte.y, &testbyte.y);
	printf_semi("testbyte.z = %d [0x%04x] \t[address = %p]\n", testbyte.z, testbyte.z, &testbyte.z);

	// With padding, size should be 8 bytes.
	printf_semi("size = %d, expected 8 bytes\nbase addr = %p\n", sizeof(testbyte), &testbyte);

	for (int i = 0; i < sizeof(testbyte); i++)
	{
		printf_semi("\t[%d][%p] = 0x%02x\n", i, &((uint8_t*)&testbyte)[i],((uint8_t*)&testbyte)[i]);
	}




}


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

					/// Test for combination tx/rx buffer
					uint8_t spi_txrx[2] = { 0, 0 };
					spi_txrx[0] = LSM330DLC_FormatAddress(true, false, LSM330DLC_REG_WHO_AM_I_G);

					// Attempt combination TX/RX
					if (HAL_SPI_TransmitReceive(IMUDevice[imuport].hspi, spi_txrx, spi_txrx, 2, SPI_TIMEOUT) != HAL_OK)
					{
						// No! Bad! Bad STM32! 
						printf_semi("SPI IMU Test (transmit/receive) failed.\n");
				
					}

					// Reset \CS
					SelectIMUSubDevice(imuport, IMU_SUBDEV_NONE);

					// ...Result should equal 0b11010100
					if (spi_txrx[1] == LSM330DLC_WHO_AM_I_G_VALUE)
					{
						// Ready to go!
						printf_semi("SPI IMU Test successful. (TxRx buffer: 0x%02x 0x%02x).\n", spi_txrx[0], spi_txrx[1]);

						return true;
						
					} 
					else
					{
						// Ready to go!
						printf_semi("SPI IMU Test unsuccessful (received 0x%02x 0x%02x).\n", spi_txrx[0], spi_txrx[1]);
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

#ifdef DEBUG_MONITOR_SPI6_ONBOARD
	// Mirror onto PB4/PB5 to use a logic analyzer connected to SPI6 to monitor onboard SPI communications. (EVP4)
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (IMU_SUBDEV_ACC == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (IMU_SUBDEV_GYRO == component) ? GPIO_PIN_RESET : GPIO_PIN_SET);		// Active-low.
#endif

}



void SetupIMU(void)
{
	
	/// @bug - I don't know if this is necessary on arm-eabi....
	memset(IMUDevice, 0, sizeof(IMUDevice));

	/// @bug - I don't know if this is necessary on arm-eabi....
	memset(&IMU_TransferState, 0, sizeof(IMU_TransferState));
	
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

void GetIMUFrame(IMU_PortType finger)
{
	// Fill frame w/IMU data
}

/// Checks For Interrupt Events
void IMU_HandleInterruptEvents(void)
{
	// For the current frame
		// Check for 'idle' imu pairs.
			// Polling: kick-off a polling DMA transfer to check status
			// Interrupt: kick-off a DMA read
}

/// Moves to next transfer
void IMU_HandleSPIEvent(void)
{
	// Determine selected imu and subdevice
	
	// Per state
	switch (0)
	{
	case IMU_XFER_IDLE:
		// A polling event was just completed.

		break;

	case IMU_XFER_PENDING:
		// Poll/Interrupt has determined that data is available

		break;

	case IMU_XFER_WAIT:
		// Data transfer was in progress. 

		break;

	case IMU_XFER_COMPLETE:
		// A polling event was just completed.

		break;

	default:
		break;
	}



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

				// Power on.
			configurationPacket[1] = LSM330DLC_CTRL_REG1_A_ODR_100HZ;
			
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
				printf_semi("ConfigureIMU(%d), IMU acc setup failed (%d)\n", finger, result);

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

			// CTRL_REG1_G - power on.
			configurationPacket[1] = LSM330DLC_CTRL_REG1_G_ODR_95HZ_BW25;


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
				printf_semi("ConfigureIMU(%d) - IMU gyro setup failed (%d)\n", finger, result);
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

IMU_PortType GetIMU(SPI_HandleTypeDef* hspi)
{
	/// @bug This may need to be manually unrolled.	Analyze/profile this!
	/*
	intptr_t ihspi = (intptr_t)(void *)hspi;	// I know what you're thinking...

	switch (ihspi)
	{
	case (intptr_t)(void*)&hspi6:
		return IMU_ONBOARD;
		break;
	default:
		/// Unrecognized hspi
		printf_semi("GetIMU(%p) - IMU not found/implemented", (void *)hspi);
		break;
	}
	*/

	// Select the requested component
	for (int i = 0; i < IMU_LAST; i++)
	{
		// Disable
		
		if (IMUDevice[i].hspi == hspi)
		{
			return i;
		}
	}

	printf_semi("GetIMU(%p) - IMU not found/implemented.", (void*)hspi);
#ifdef DEBUG
	__BKPT(0);
#endif

	return IMU_LAST;


}



