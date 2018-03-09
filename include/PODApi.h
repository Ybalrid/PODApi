#pragma once

#include "packets.h"
#include "PODdll.h"

typedef void(__stdcall* unity_debug_callback)(const char* str);

int unity_printf(const char* str, ...);

POD_EXPORT int test_me();
POD_EXPORT void get_vector(float* out);

POD_EXPORT void register_debug_callback(unity_debug_callback callback);

POD_EXPORT void POD_init();
POD_EXPORT void POD_update();
POD_EXPORT void POD_exit();
