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
	s->server.sin_port		  = htons(POD_PROTOCOL_PORT);
	if(bind(s->socket, (struct sockaddr*)&s->server, sizeof(s->server)) == SOCKET_ERROR)
	{
		puts("SOCKET_ERROR");
		MessageBoxA(NULL, "bind() failed on POD_Internal_initNetwork_WIN32", "bind() failed!", MB_ICONERROR);
		//TODO handle error
	}

	s->packet_source_len = (int)sizeof s->packet_source;
	s->send_socket		 = socket(AF_INET, SOCK_DGRAM, 0);
	int broadcast		 = 1; //Authorize this socket to broadcast
	setsockopt(s->send_socket, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof broadcast);

	struct sockaddr_in si_broadcast;
	memset((char*)&si_broadcast, 0, sizeof(si_broadcast));
	si_broadcast.sin_family		 = AF_INET;
	si_broadcast.sin_port		 = htons(POD_PROTOCOL_PORT);
	si_broadcast.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	struct POD_message helloMessage;
	helloMessage.packetType   = PODAPI_HELLO;
	static const char hello[] = "Hi there! I'm some program using PODApi, and I say HELLO! :D";
	strcpy_s(helloMessage.text, sizeof helloMessage.text, hello);

	char i;
	for(i = 0; i < 3; ++i)
	{
		sendto(s->send_socket,
			   (char*)&helloMessage,
			   sizeof helloMessage,
			   0,
			   (struct sockaddr*)&si_broadcast,
			   sizeof si_broadcast);
		Sleep(33);
	}
}

void POD_Internal_closeNetwork_WIN32(state* s)
{
	closesocket(s->socket);
	WSACleanup();
}

size_t recv_all(state* s)
{
	static size_t received;
	static int status;
	received = 0;

	do
	{
		//u_long bytes_available;
		//ioctlsocket(s->socket, FIONREAD, &bytes_available);
		//POD_printf("Available : %d\n", bytes_available);

		//IMPORTANT : this assume that there's only one POD client talking to us. Last one to send a packet will receive awnser (if we send any)
		status = recvfrom(s->socket, (char*)s->buffer + received, (int)(sizeof s->buffer - received), 0, (struct sockaddr*)&s->packet_source, &s->packet_source_len);

		if(status > 0)
			received += status;

	} while(status > 0);

	return received;
}

void POD_Internal_processIncomingData_WIN32(state* s)
{
	static int size;
	size = (int)recv_all(s);

	if(size > 0)
	{
		POD_Byte* data = s->buffer;

		while(size > 0)
		{
			fprintf(stderr, "packet type: = 0x%x\n", data[0]);
			switch(data[0])
			{
				case PODAPI_SERVER_ASK:
				{
					static const int expectedSize = (int)sizeof(struct POD_ServerAsk);
					if(size < expectedSize)
					{
						POD_printf("packet buffer problem\n");
						size = 0;
						break;
					}

					//packet is a one byte empty message we don't care about, but we are going to awnser the server_ask with a server_ok
					struct POD_ServerOk server_ok_packet;
					server_ok_packet.packetType = PODAPI_SEVERHERE;

					//This thing should point to the packet origin, just overwrite the incomming port with POD_PROTOCOL_PORT
					s->packet_source.sin_port = htons(POD_PROTOCOL_PORT);

					sendto(s->send_socket,
						   (char*)&server_ok_packet,
						   (int)sizeof server_ok_packet,
						   0,
						   (struct sockaddr*)&s->packet_source,
						   (int)s->packet_source_len);

					//Advance on the buffer
					data += expectedSize;
					size -= expectedSize;
				}
				break;
				case PODAPI_WALK_VEC:
				{
					static const int expectedSize = (int)sizeof(struct POD_WalkVector);
					if(size < expectedSize)
					{
						POD_printf("packet buffer problem\n");
						size = 0;
						break;
					}

					struct POD_WalkVector* packet = (struct POD_WalkVector*)data;

					s->mostRecentTime = packet->timepoint;
					s->podWalkX		  = packet->x;
					s->podWalkY		  = packet->y;

					data += expectedSize; //Advance the pointer
					size -= expectedSize; //Remove the read data from the recv data.
				}
				break;
				default:
					POD_printf("Something went wrong while reading packets!\n");
					//Unrecognized packet type, break loop:
					size = 0;
					break;
			}
		}
	}
}

#endif
