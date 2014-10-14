/// @file WiFi.h CC3x00 Wireless Networking Support

//////////////////////////////////////////////////////////////////////////
#include "stm32f4xx_hal.h"



/// Connection State
typedef enum
{
	WIFI_UNCONFIGURED,	/// WiFi hasn't yet been configured with an SSID/Password.
	WIFI_DISCONNECTED,	/// Wifi is configured, but isn't connected to an AP (nor is it in Direct Mode)  No communications.
	WIFI_IDLE,			/// Connected to an AP/Direct, but no socket/connection. 
	WIFI_LISTENING,		/// Listening on a UDP socket, (will occasionally broadcast presence.)
	WIFI_CONNECTED		/// Connected to an AP/Direct, has a UDP socket with an active data connection from a host. 

} WIFIStateType;

/// Configuration