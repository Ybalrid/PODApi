#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <packets.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, char* argv[])
{
	struct POD_WalkVector vector;
	vector.timepoint	  = 0;
	vector.packetType	 = PODAPI_WALK_VEC;
	const float walkSpeed = 3.f;
	WSADATA wsa;
	SOCKET VR_App_Socket;
	struct sockaddr_in si_other;
	int slen = sizeof si_other;

	char* server = argc > 1 ? argv[1] : "127.0.0.1";

	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	if((VR_App_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family		 = AF_INET;
	si_other.sin_port		 = htons(POD_PROTOCOL_PORT);
	si_other.sin_addr.s_addr = inet_addr(server);

	LARGE_INTEGER start, stop, freq, elapsed;
	QueryPerformanceCounter(&start);
	while(true)
	{
		Sleep(40);

		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&stop);

		elapsed.QuadPart = stop.QuadPart - start.QuadPart;
		elapsed.QuadPart *= 1000000;
		elapsed.QuadPart /= freq.QuadPart;

		//Generate some stuff
		vector.timepoint += elapsed.QuadPart;

		//This just describe a circle, to test if movement information can be sent
		vector.x = walkSpeed * sin((double)elapsed.QuadPart / 1000000.0);
		vector.y = walkSpeed * cos((double)elapsed.QuadPart / 1000000.0);

		sendto(VR_App_Socket,
			   (char*)&vector,
			   sizeof vector,
			   0,
			   (struct sockaddr*)&si_other,
			   slen);
		printf("Sent with timepoint %lld\n", vector.timepoint);
	}

	closesocket(VR_App_Socket);
	WSACleanup();
}
