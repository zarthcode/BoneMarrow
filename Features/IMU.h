/// @file IMU.h IMU SPI driver declarations

#include "stm32f4xx_hal.h"
#include "spi.h"
#include <stdbool.h>
#include "LSM330DLC.h"
#include "Spatial.h"


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
//	IMU_DEVICE_LSM6DS0,
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
	IMU_PinMappingType CSMappings[IMU_SUBDEV_NONE];	/// @bug this needs to be configurable based on hardware and detected IMU
	IMU_PinMappingType INTMappings[IMU_SUBDEV_NONE];	/// @bug this needs to be configurable based on hardware and detected IMU
	IMU_ComponentType IMUType;
	float FullScale[IMU_SUBDEV_NONE];
} IMU_MappingStruct;

typedef struct 
{
	IMU_RAW imu[IMU_LAST];
} IMU_RAWFrame;

// IMU Map
extern IMU_MappingStruct IMUDevice[IMU_LAST];

/// Total number of RAW buffers.
#define IMU_FRAMEBUFFER_SIZE 2

/// IMU Framebuffer
extern IMU_RAWFrame IMU_RAWFramebuffer[IMU_FRAMEBUFFER_SIZE];		/// @todo Implement FIFO 

extern uint8_t volatile IMU_RAWFramebuffersPending;
extern uint8_t volatile IMU_RAWFramebuffer_ReadIndex;
extern uint8_t volatile IMU_RAWFramebuffer_WriteIndex;


// IMU Frame State
typedef enum
{
	IMU_FRAME_UNLOCKED,			// No data. Waiting for event. (no lock)
	IMU_FRAME_WRITELOCK,		// Frame is currently being assembled.	(write lock)
	IMU_FRAME_READLOCK			// Application is reading data from this frame.	(read lock)
} IMU_FrameLockType;

// Data transfer steps
typedef enum
{
	IMU_XFER_IDLE,				// Waiting for trigger (poll, interrupt)
	IMU_XFER_CHECKING,			// A polling operation is in progress.
	IMU_XFER_PENDING,			// Data ready for transfer (interrupt/poll confirmed)
	IMU_XFER_WAIT,				// Data transfer in progress
	IMU_XFER_COMPLETE			// Data transfer completed.
} IMU_TransferStepType;


/// @todo IMU_POLL_BUFFER_SIZE is an excellent candidate for malloc/free.
#define IMU_POLL_BUFFER_SIZE 2	// Only 2 bytes needed to hold TxRx polling transfers.

/// Transfer State
typedef struct 
{
	IMU_FrameLockType Lock;
	IMU_TransferStepType TransferStep[IMU_LAST][IMU_SUBDEV_NONE];	
	uint8_t PollingBuffer[IMU_LAST][IMU_POLL_BUFFER_SIZE];
	IMU_SubDeviceType SelectedSubDevice[IMU_LAST];
} IMU_TransferStateType;

/// Checks For Interrupt Events
void IMU_CheckIMUInterrupts(void);

/// Initiates DMA transfer to grab data info from device.
void IMU_Poll(IMU_PortType port, IMU_SubDeviceType subdev);

/// Evaluates the result of the polling operation.
IMU_TransferStepType IMU_CheckPollingResult(IMU_PortType port, IMU_SubDeviceType subdev);

/// Moves to next transfer
void IMU_HandleSPIEvent(IMU_PortType port);

/// Starts appropriate DMA transfer on the given port
void IMU_HandleTransfer(IMU_PortType port);

/// Start DMA transfer of data from specified port/subdevice.
void IMU_GetRAW(IMU_PortType port, IMU_SubDeviceType subdev);

/// A special case DMA transfer that takes place after a successful polling operation.
void IMU_GetRAWBurst(IMU_PortType port, IMU_SubDeviceType subdev);


/// Performs initial setup of IMU mapping structures
/// @todo Rewrite IMU methods using a driver approach. IMU_Setup should detect/configure devices.
void IMU_Setup(void);

/// Returns a multiplier for the currently-configured full-scale setting.
float IMU_GetFullScaleMultiplier(IMU_PortType port, IMU_SubDeviceType subdev);

/// Evaluates FullScale setting for each port, based on the last few frames.
void IMU_AutoSetFullScaleSetting(IMU_PortType port, IMU_SubDeviceType subdev);

/**
 * @brief Attempts to force a particular full-scale setting.
 * @returns final full-scale setting.
 * @param IMU_PortType port IMU device descriptor 
 * @param IMU_SubDeviceType subdev IMU subdevice
 * @todo Change this routine to utilize a device driver architecture.
 */
float IMU_ForceFullScaleSetting(IMU_PortType port, IMU_SubDeviceType subdev, float g_max);

/// Utility function that returns the imu associated w/an hspi
IMU_PortType IMU_GetPortFromSPIHandle(SPI_HandleTypeDef* hspi);

/// Selects the requested component
void IMU_SelectSubDevice(IMU_PortType port, IMU_SubDeviceType component);

/// Detect connected IMU(s)
// IMU_ComponentType DetectIMU(IMU_MappingStruct* device);

/// Configures the selected IMU
/**
 * @brief 
 * @todo This function needs to be a driver arch.
 * @todo settings/configuration needs to be be cached into IMUDevice structure.
 * @param port IMU of the device to configure.
 */
void IMU_Configure(IMU_PortType port);

/// Processes Completed/pending IMU_RAW frames into IMU_SCALED frames and frees an IMU_Framebuffer
void IMU_ProcessRAWFrame(void);

/// Processes scaled imu data into quaternions
void IMU_ProcessOrientation(void);

/// Removes common (world) motion/acceleration from the frame.  Leaving the relative 

/// Performs IMU service once per mS
void IMU_SystickHandler(void);
 
/// Determines if the SPI port can communicate with the IMU.
bool DIAG_IMU_Test(IMU_PortType imuport);

/// Debug/Dev function to check structure alignment
bool DIAG_IMU_CheckAlignment(void);


/**
 * @brief Updates the specified quaternion using the provided IMU data using Madgwick's AHRS Algorithm.
 * @returns void
 * @param[in|out]	pQ		The output quaternion to be updated.
 * @param[in]		pIMU	IMU data to be used to update the quaternion.
 * @param[in]		ODR		Refresh rate of the IMU.
 * @param[in]	   	beta	"2 * Proportional Gain (Kp)"
 */
void IMU_QuaternionUpdate(Quaternion* pQ, IMU_SCALED* pIMU, float ODR, float beta);

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


// AHRS Algorithm(s)

// Madgwick AHRS (http://www.x-io.co.uk/open-source-imu-and-ahrs-algorithms/)
// Optimized for ARM Cortex M4F