/// @file Semihosting.h  ARM Semihosting debug methods.

/// @todo Add a debug initialize function that performs redirect to different places.

#ifdef DEBUG
#ifdef SEMIHOSTING
#include <stdio.h>	// Practically nothing else from here is in use. funny enough.

#define printf_semi(...) printf(__VA_ARGS__)
#else

// Non-semihosting debug redirect (USB? UART? I2C?)

#define printf_semi(...)

#endif
#else // DEBUG

// Release (optimized. no redirects)

#define printf_semi(...)


#endif

