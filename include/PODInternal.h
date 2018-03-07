#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
//Here you include inet, arpa stuff and other linuxy thingies...
#endif
#include <stdio.h>

typedef struct state_
{
#ifdef _WIN32
	WSADATA wsa;
	SOCKET socket;
	struct sockaddr_in server, si_other;
#else
	//Linux/Android NDK here
#endif
} state;

void POD_Internal_initNetwork(state* s);
