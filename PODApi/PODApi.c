#include "packets.h"
#include "PODApi.h"
#include "PODdll.h"
#include "PODInternal.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Some data
float vect[3] = { 1, 2, 3 };

state* s = NULL;

POD_EXPORT int test_me()
{
	return 42;
}

//Will write 3 floats into the "out" pointer
POD_EXPORT void get_vector(float* out)
{
	//memcpy the data inside the output
	memcpy(out, vect, sizeof(float) * 3);
}

POD_EXPORT void POD_init()
{
	if(s)
	{
		fprintf(stderr, "Library was already initialized. This is a fatal error! Please call POD_init only once!");
		exit(1);
	}

	s = (state*)malloc(sizeof(state));

	POD_Internal_initNetwork(s);
}

POD_EXPORT void POD_update()
{
}

POD_EXPORT void POD_exit()
{
	free(s);
	s = NULL;
}
