/// @file TI CC3000 USART3/SPI Implementation
#include <stdint.h>
#include <stdbool.h>

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


/**
 * @brief Called after the received packet is processed by the CC3000 host driver code
 *  in the context of the receive handler.
 */
void SpiResumeSpi();

long WLAN_ReadIRQPin(void);

void WLAN_IRQEnable(void);

void WLAN_IRQDisable(void);

void WLAN_nIRQ_Event(void);

void WLAN_USART_TxRxComplete(void);

void WLAN_Service(void);

/// \note Interrupt will occur, once the CC3000 is ready. (approx 53ms from enabling.)
void WLAN_WriteENPin(unsigned char val);

extern uint8_t wlan_tx_buffer[];			// WLAN Transmit buffer
extern uint8_t wlan_rx_buffer[];			// WLAN Receive buffer

extern volatile bool wlan_rx_pending;
extern volatile bool wlan_irq_enabled;
extern volatile bool wlan_irq_flag;