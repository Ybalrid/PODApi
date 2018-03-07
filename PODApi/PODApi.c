#include "packets.h"
#include "PODApi.h"
#include "PODdll.h"
#include <string.h>

//Some data
float vect[3] = { 1, 2, 3 };

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
}

POD_EXPORT void POD_update()
{
}

POD_EXPORT void POD_exit()
{
}
