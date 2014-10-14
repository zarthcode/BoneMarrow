#include <stdint.h>

/// uS Delay function
void Delay_Init(void);

void Delay_uS(uint32_t us);

/// Returns uS timer value.
uint32_t DWT_Get(void);