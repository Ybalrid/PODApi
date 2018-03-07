#include "packets.h"
#include "PODApi.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
int main()
{
	const float running_time = 10.f;
	POD_init();
	getchar();
	printf("running loop for %f seconds...", running_time);

	bool running  = true;
	clock_t start = clock();
	while(running)
	{

		POD_update();

		clock_t end   = clock();
		const float s = (float)(end - start) / CLOCKS_PER_SEC;
		if(s > running_time) running = false;
	}

	POD_exit();

	return 0;
}
