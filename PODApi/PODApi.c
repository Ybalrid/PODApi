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

int unity_printf(const char* str, ...)
{
	if(!s->unity_log_print_fptr) return 0;

	//Unpack variadic
	va_list args;
	va_start(args, str);

	//Calcuate size of string
	size_t need = vsnprintf(NULL, 0, str, args);

	//Build string
	char* buffer = malloc(need);
	int printed  = vsnprintf(buffer, need, str, args);

	//Print to unity's log :
	s->unity_log_print_fptr(buffer);

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

	POD_Internal_initNetwork(s);
}

POD_EXPORT void POD_update()
{
	unity_printf("update : %s %d %x", "Hello this is a string", 42, 0x42);
}

POD_EXPORT void POD_exit()
{
	POD_Internal_closeNetwork(s);
	free(s);
	s = NULL;
}
