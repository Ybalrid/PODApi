#include "PODInternal.h"

#ifdef _WIN32
#include "PODInternal_WIN32.h"
#else
#endif

#include "packets.h"

void POD_Internal_initNetwork(state* s)
{
#ifdef _WIN32
	return POD_Internal_initNetwork_WIN32(s);
#else
	return POD_Internal_initNetwork_POSIX(s);
#endif
}

void POD_Internal_closeNetwork(state* s)
{
#ifdef _WIN32
	POD_Internal_closeNetwork_WIN32(s);
#else
#endif
}

POD_Byte buffer[128];

void POD_Internal_processIncomingData(state* s)
{

#ifdef _WIN32
	POD_Internal_processIncomingData_WIN32(s);
#else
#endif


}
