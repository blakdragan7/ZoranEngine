#pragma once

#define AE_NO_ERROR 0

/* Usually returned when some operation was attempted that isn't supported. 
such as turning on a capability that this engine implementation doesnt supprot*/
#define AE_NOT_SUPPORTED 1
/* Returned when In invalid state (like playing audio with no listeners) */
#define AE_INVALID_OPERATION 2
/* Returned if an audio function is called without calling Init first */
#define AE_NOT_INITIZLED 3