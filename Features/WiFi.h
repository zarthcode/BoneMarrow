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

void WiFi_Configure(void);	/// Startup WiFi Configuration

WIFIStateType WiFi_State(void);		/// Returns the current WiFi state.

void WiFi_SetState(WIFIStateType state);	/// Attempts to force wifi into the requested state

void WiFi_HandleState(void);		/// Handles a change in state.

void WiFi_Callback(long event_type, char * data, unsigned char length);


	typedef struct
	{
		uint8_t a;
		uint8_t b;
		uint8_t c;
		uint8_t d;
	} ip_addr;

/// DHCP Report
typedef struct
{
	ip_addr ip;
	ip_addr mask;
	ip_addr gateway;
	ip_addr dhcp_server;
	ip_addr dns_server;
} Dhcp_ReportType;

/// Ping Report
typedef struct
{
	uint32_t packets_sent;
	uint32_t packets_received;
	uint32_t min_round_time;
	uint32_t max_round_time;
	uint32_t avr_round_time;
} Ping_ReportType;