/// @file IMU.h IMU SPI declarations

#include "stm32f4xx_hal.h"
#include "spi.h"
#include <stdbool.h>
#include "LSM330DLC.h"


#define SPI_TIMEOUT 200

/// @brief Finger Port list
/// @todo What about (beta-series) advanced fingers w/brains?
typedef enum
{
	IMU_ONBOARD,		// This is the onboard IMU, normally placed on the forearm.
	IMU_P1,
	IMU_P2,
	IMU_P3,
	IMU_P4,
	IMU_P5,
	IMU_P6,
	IMU_LAST			// Iteration and array setup aid.

} IMU_PortType;

typedef enum
{
	Finger_Alpha
//	Finger_Beta
} FingerDeviceType;


/// @brief Each IMU contains several devices that are often accessed separately.
typedef enum
{
	IMU_SUBDEV_ACC,
	IMU_SUBDEV_GYRO,
	IMU_SUBDEV_NONE		// Iteration aid.
} IMU_SubDeviceType;

/// Interrupt and  \CS Mapping
typedef struct
{
	GPIO_TypeDef* port;	// Port
	uint16_t pin;		// Pin
	IMU_SubDeviceType type;	// Accelerometer or gyroscope.
} IMU_PinMappingType;

/// Device types
/// @todo Consider external i2c device access, barometers
typedef enum
{
	IMU_DEV_6AXIS,
	IMU_DEV_9AXIS

} IMU_ComponentType;

/// @brief This is meant for auto-detection, implementation of multiple MEMS sensor types.
typedef enum
{
	IMU_DEVICE_LSM330DLC,		// EVP4, Alpha-0.
//	IMU_DEVICE_LSM9DS0,
//	IMU_DEVICE_LSM6DB0,
//	IMU_DEVICE_LSM330,
	IMU_DEVICE_LAST
} IMU_Device;


/// @brief Information needed to access a single IMU
typedef struct  
{
	IMU_Device DeviceName;
	SPI_HandleTypeDef* hspi;
	bool hasMagnetometer;
	IMU_PinMappingType CSMappings[2];	/// @bug this needs to be configurable based on hardware and detected IMU
	IMU_PinMappingType INTMappings[4];	/// @bug this needs to be configurable based on hardware and detected IMU
	IMU_ComponentType IMUType;
} IMU_MappingStruct;


/// Performs initial setup of IMU mapping structures
void SetupIMU(void);

/// Determines if the SPI port can communicate with the IMU.
bool IMUTest(IMU_PortType imuport);


/// Selects the requested component
void SelectIMUSubDevice(IMU_PortType finger, IMU_SubDeviceType component);

/// Detect connected IMUs and
IMU_ComponentType DetectIMU(IMU_MappingStruct* device);

/// Configures Interrupt Pins
void IMU_ConfigureEXTI(IMU_PinMappingType* pinmap);

/// Retrieves IMU data as a DMA transfer

/*

	Interrupt Mapping (EVP4)
	SPI1 RESV0	-	PA3
	SPI1 RESV1	-	PA4
	SPI1 INT0	-	PH4
	SPI1 INT1	-	PH5

	SPI2 RESV0	-	PG2
	SPI2 RESV1	-	PG3
	SPI2 INT0	-	PD10
	SPI2 INT1	-	PD11
	
	SPI3 RESV0	-	PH14
	SPI3 RESV1	-	PH15
	SPI3 INT0	-	PD2
	SPI3 INT1	-	PH13
	
	SPI4 RESV0	-	PE4
	SPI4 RESV1	-	PI8
	SPI4 INT0	-	PC13
	SPI4 INT1	-	PI9

	SPI5 RESV0	-	PF6
	SPI5 RESV1	-	PF10
	SPI5 INT0	-	PF4
	SPI5 INT1	-	PF5

	SPI6 RESV0	-	PI4
	SPI6 RESV1	-	PE3
	SPI6 INT0	-	PD7
	SPI6 INT1	-	PG9
	SPI6 INT2	-	PG10
	SPI6 INT3	-	PG11
	SPI6 INT4	-	PE0
	SPI6 INT5	-	PE1
	

*/