#include "PODInternal.h"
#include "PODInternal_WIN32.h"
#ifdef _WIN32

void POD_Internal_initNetwork_WIN32(state* s)
{
	//Init winsock
	if(WSAStartup(MAKEWORD(2, 2), &s->wsa) != 0)
	{
		puts("Error when WSAStartup");
		//TODO handle error
	}

	//Create socket
	if((s->socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		puts("INVALID_SOCKET");
		//TODO handle error
	}

	//Configure to listen for incoming UDP datagrams on port 4242
	s->server.sin_family	  = AF_INET;
	s->server.sin_addr.s_addr = INADDR_ANY;
	s->server.sin_port		  = htons(4242);
	if(bind(s->socket, (struct sockaddr*)&s->server, sizeof(s->server)) == SOCKET_ERROR)
	{
		puts("SOCKET_ERROR");
		//TODO handle error
	}
}

#endif
