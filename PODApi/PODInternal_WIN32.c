#include "PODInternal.h"
#include "PODInternal_WIN32.h"
#ifdef _WIN32

void POD_Internal_initNetwork_WIN32(state* s)
{

	printf("This is a test!\n");
	//Init winsock
	if(WSAStartup(MAKEWORD(2, 2), &s->wsa) != 0)
	{
		puts("Error when WSAStartup");
		//TODO handle error
		MessageBoxA(NULL, "WSAStartup failed on POD_Internal_initNetwork_WIN32", "WSAStartup failed!", MB_ICONERROR);
	}

	//Create socket
	if((s->socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		puts("INVALID_SOCKET");
		MessageBoxA(NULL, "socket() failed on POD_Internal_initNetwork_WIN32", "socket() failed!", MB_ICONERROR);
		//TODO handle error
	}

	u_long noblock = 1;
	if(ioctlsocket(s->socket, FIONBIO, &noblock) != NO_ERROR)
	{
		puts("ioctlsocket ERROR");
		MessageBoxA(NULL, "ioctlsocket() failed on POD_Internal_initNetwork_WIN32", "ioctlsocket() failed!", MB_ICONERROR);
	}

	//Configure to listen for incoming UDP datagrams on port 4242
	s->server.sin_family	  = AF_INET;
	s->server.sin_addr.s_addr = INADDR_ANY;
	s->server.sin_port		  = htons(4242);
	if(bind(s->socket, (struct sockaddr*)&s->server, sizeof(s->server)) == SOCKET_ERROR)
	{
		puts("SOCKET_ERROR");
		MessageBoxA(NULL, "bind() failed on POD_Internal_initNetwork_WIN32", "bind() failed!", MB_ICONERROR);
		//TODO handle error
	}
}

void POD_Internal_closeNetwork_WIN32(state* s)
{
	closesocket(s->socket);
	WSACleanup();
}

POD_Byte buffer[128];
void POD_Internal_processIncomingData_WIN32(state* s)
{
	int status = recv(s->socket, buffer, 128, 0);
	if(status < 0)
	{
		if(status == WSAEWOULDBLOCK) //Windows specific, create dispatcher function
		{
			puts("no data.");
			return;
		}

		if (status == WSAEMSGSIZE)
		{
			POD_printf("Internal network buffer overlow. You need to call POD_update() more often!");
		}
	}

	if(status == 0)
	{
		puts("no data.");
	}

	if(status > 0)
	{
		int size	= status;
		POD_Byte* data = buffer;

		while(size > 0)
		{
			switch(data[0])
			{
				case PODAPI_WALK_VEC:
				{
					static const size_t expectedSize = sizeof(struct POD_WalkVector);
					if(size < expectedSize)
					{
						POD_printf("packet buffer problem");
						size = 0;
						break;
					}

					struct POD_WalkVector* packet = (struct POD_WalkVector*)buffer;
					//printf("Walk vector at timepoint %lld: X=%f, Y=%f\n", packet->timepoint, packet->x, packet->y);

					s->mostRecentTime = packet->timepoint;
					s->podWalkX = packet->x;
					s->podWalkY = packet->y;

					data += expectedSize; //Advance the pointer
					size -= expectedSize; //Remove the read data from the recv data.
				}
				default:
				break;
			}
		}
	}
}

#endif
