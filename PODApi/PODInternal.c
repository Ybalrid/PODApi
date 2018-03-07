#include "PODInternal.h"

#ifdef _WIN32
#include "PODInternal_WIN32.h"
#else
#endif

void POD_Internal_initNetwork(state* s)
{
#ifdef _WIN32
	return POD_Internal_initNetwork_WIN32(s);
#else
	return POD_Internal_initNetwork_POSIX(s);
#endif
}
