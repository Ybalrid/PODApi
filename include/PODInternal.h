#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
//Here you include inet, arpa stuff and other linuxy thingies...
#endif

#include <stdio.h>
#include "PODApi.h"

typedef struct state_
{
	unity_debug_callback unity_log_print_fptr;
#ifdef _WIN32
	WSADATA wsa;
	SOCKET socket;
	struct sockaddr_in server, si_other;
#else
	//Linux/Android NDK here
#endif
} state;

void POD_Internal_initNetwork(state* s);
void POD_Internal_closeNetwork(state* s);
