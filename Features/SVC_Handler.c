#include "SVC_Handler.h"

// Unimplemented, for now.
void SVC_Handler(void)
{

	#ifdef DEBUG
	printf_semi("SVC_Handler called. (Unimplemented - HALT)\n");
__BKPT(0);
	#endif
	while (1) {};

}
