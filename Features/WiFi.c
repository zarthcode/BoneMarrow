/// @file WiFi.c CC3x00 WiFi Implementation

#include "WiFi.h"
#include "CC3000_Impl.h"
#include "hci.h"
#include "wlan.h"
#include "Semihosting.h"

#include <inttypes.h>


// Wifi state
WIFIStateType WiFi_CurrentState = WIFI_UNCONFIGURED;

void WiFi_Configure(void)
{

	if (WiFi_CurrentState != WIFI_UNCONFIGURED)
	{
		return;
	}


	// Setup Callbacks
	

	wlan_init(
		WiFi_Callback,		// Asynchronous event callback
		0,					// FW Patch not implemented.
		0,					// Driver patch not implemented.
		0,					// Bootloader patch not implemented.
		&WLAN_ReadIRQPin,
		&WLAN_IRQEnable,
		&WLAN_IRQDisable,
		&WLAN_WriteENPin);

		wlan_start(0);

	// First-time setup

		// AP Configuration

	// Configuration Complete
	WiFi_CurrentState = WIFI_DISCONNECTED;

}

void WiFi_Callback(long event_type, char * data, unsigned char length)
{
	switch (event_type)
	{
	case HCI_EVNT_WLAN_UNSOL_CONNECT:
		// Connect event
		printf_semi("WLAN HCI EVENT - Unsolicited connect\n");
		break;
	case HCI_EVNT_WLAN_UNSOL_DISCONNECT:
		// Disconnect event
		printf_semi("WLAN HCI EVENT - Unsolicited disconnect\n");
		break;
	case HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE:
		// Configuration done
		printf_semi("WLAN HCI EVENT - Asynchronous Simple Config Done\n");
		break;
	case HCI_EVNT_WLAN_KEEPALIVE:
		// Keepalive event
		printf_semi("WLAN HCI EVENT - Keepalive\n");
		break;
//	Events with extra data:
	case HCI_EVNT_WLAN_UNSOL_DHCP: 
		// DHCP report
		// 4 bytes IP, 4 bytes Mask, 4 bytes default gateway, 
		// 4 bytes DHCP server and 4 bytes for DNS server.
		printf_semi("WLAN HCI EVENT - DHCP\n");
		Dhcp_ReportType* dhcp = (Dhcp_ReportType*)data;
		printf_semi("\tip: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n \tmask: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n\tgateway: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n\tdhcp server: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n\tdns server: %" PRIu8 ".%" PRIu8 ".%" PRIu8 ".%" PRIu8 "\n\n",
			dhcp->ip.a, dhcp->ip.b, dhcp->ip.c, dhcp->ip.d,
			dhcp->mask.a, dhcp->mask.b, dhcp->mask.c, dhcp->mask.d,
			dhcp->gateway.a, dhcp->gateway.b, dhcp->gateway.c, dhcp->gateway.d,
			dhcp->dhcp_server.a, dhcp->dhcp_server.b, dhcp->dhcp_server.c, dhcp->dhcp_server.d,
			dhcp->dns_server.a, dhcp->dns_server.b, dhcp->dns_server.c, dhcp->dns_server.d);
		break;
	case HCI_EVNT_WLAN_ASYNC_PING_REPORT: 
		// Ping Report
		// 4 bytes Packets sent, 
		// 4 bytes Packets received, 4 bytes Min round time, 
		// 4 bytes Max round time and 4 bytes for Avg round time.
		printf_semi("WLAN HCI EVENT - Ping\n");
		Ping_ReportType* ping = (Ping_ReportType*)data;
		printf_semi("\tPackets Sent: %" PRIu32 "\n\tPackets Received: %" PRIu32 "\n\tMin Round Time: %" PRIu32 "\n\tMax Round Time: %" PRIu32 "\n\tAvg Round Time: %" PRIu32 "\n\n",
			ping->packets_sent, ping->packets_received, ping->min_round_time, ping->max_round_time, ping->avr_round_time);
		break;
	default:
		printf_semi("WLAN HCI EVENT - Unrecognized(%" PRIu32 ")\n", event_type);
		break;
	}
}
