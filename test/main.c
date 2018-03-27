#include "packets.h"
#include "PODApi.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

int main()
{
	const float running_time = 300.f;
	POD_init();
	getchar();
	printf("running loop for %f seconds...", running_time);

	bool running  = true;
	clock_t start = clock();

	float v[3] = { 0 };
	while(running)
	{
		Sleep(10);
		POD_update();

		POD_get_walk_linear_speed_vector(v);
		printf("speed vector time %lld exposed to game engine : %f, %f, %f\n", POD_get_most_recent_time_code(), v[0], v[1], v[2]);

		clock_t end   = clock();
		const float s = (float)(end - start) / CLOCKS_PER_SEC;
		if(s > running_time) running = false;
	}

	POD_exit();

	return 0;
}
