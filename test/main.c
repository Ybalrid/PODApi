#include "packets.h"
#include "PODApi.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
int main()
{
	const float running_time = 120.f;
	POD_init();
	getchar();
	printf("running loop for %f seconds...", running_time);

	bool running  = true;
	clock_t start = clock();
	
	float v[3] = {0};
	while(running)
	{

		POD_update();

		POD_get_walk_linear_speed_vector(v);
		printf("speed vector exposed to game engine : %f, %f, %f\n", v[0], v[1], v[2]);


		clock_t end   = clock();
		const float s = (float)(end - start) / CLOCKS_PER_SEC;
		if(s > running_time) running = false;
	}

	POD_exit();

	return 0;
}
