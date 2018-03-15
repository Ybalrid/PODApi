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
	SOCKET socket, send_socket;
	struct sockaddr_in server, packet_source;
	int packet_source_len;
#else
	//Linux/Android NDK here
#endif

	POD_Time mostRecentTime;
	POD_Float podWalkX, podWalkY;

	POD_Byte buffer[4096];

} state;

void POD_Internal_initNetwork(state* s);
void POD_Internal_closeNetwork(state* s);
void POD_Internal_processIncomingData(state* s);
