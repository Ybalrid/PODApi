#include "packets.h"
#include "PODApi.h"
#include "PODdll.h"
#include "PODInternal.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//Some data
float vect[3] = { 1, 2, 3 };

//Global state of the library :
state* s = NULL;

int POD_printf(const char* str, ...)
{
	//Unpack variadic
	va_list args;
	va_start(args, str);

	//Calcuate size of string
	const size_t need = vsnprintf(NULL, 0, str, args);

	//Build string
	char* buffer = (char*)malloc(need);
	const int printed  = vsnprintf(buffer, need, str, args);

	//Print to unity's log :
	if(s->unity_log_print_fptr) s->unity_log_print_fptr(buffer);
	puts(buffer);

	//Cleanup
	va_end(args);
	free(buffer);

	//Return number of char actually printed
	return printed;
}

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

POD_EXPORT void register_debug_callback(unity_debug_callback callback)
{
	s->unity_log_print_fptr = callback;
}

POD_EXPORT void POD_init()
{
	if(s)
	{
		fprintf(stderr, "Library was already initialized. This is a fatal error! Please call POD_init only once!");
		exit(1);
	}

	s						= (state*)malloc(sizeof(state));
	s->unity_log_print_fptr = NULL;
	s->mostRecentTime = -1;
	s->podWalkX = 0;
	s->podWalkY = 0;

	POD_Internal_initNetwork(s);
}

POD_EXPORT void POD_update()
{
	POD_Internal_processIncomingData(s);
}

POD_EXPORT void POD_exit()
{
	POD_Internal_closeNetwork(s);
	free(s);
	s = NULL;
}

POD_EXPORT void POD_get_walk_linear_speed_vector(float* output)
{
	//This is a 3D vector of floats
	static float walk_data_holder[3];

	//Assign the data into this array (format it as a 3D vector in horizontal plane from a 2D vector)
	walk_data_holder[0] = s->podWalkX;
	walk_data_holder[1] = 0;
	walk_data_holder[2] = s->podWalkY;

	//Write vector into the buffer provided by app or game engine (Unity...)
	memcpy(output, walk_data_holder, sizeof walk_data_holder);
}
