#pragma once
#include "PODInternal.h"

#ifdef _WIN32

void POD_Internal_initNetwork_WIN32(state* s);
void POD_Internal_closeNetwork_WIN32(state* s);

#endif
