/// @file TI CC3000 USART3/SPI Implementation
#include <stdint.h>

/**
 * @note This implementation is imperfect, latency-wise:
 *
 *		1. SPI2 is sitting on a needed DMA stream to automate DMA TX operations.
			Ideally, this should have a dedicated DMA. (Meaning, it should share a SPI port.)
 *
 */

/// SPI Receive Handler
typedef void(*CC3000_SPIRecvHandlerType)(void*);


void SpiOpen(CC3000_SPIRecvHandlerType pfRxHandler);

void SpiClose();

long SpiWrite(uint8_t* pUserBuffer, unsigned short usLength);

void SpiRead();

void SpiResumeSpi();

long sReadWlanInterruptPin(void);

void sWlanInterruptEnable(void);

void sWlanInterruptDisable(void);

void sWriteWlanPin(unsigned char val);

extern uint8_t wlan_tx_buffer[];			// WLAN Transmit buffer
extern uint8_t wlan_rx_buffer[];			// WLAN Receive buffer
