#pragma once

#include "packets.h"
#include "PODdll.h"

typedef void(__stdcall* unity_debug_callback)(const char* str);

int POD_printf(const char* str, ...);

POD_EXPORT int test_me();
POD_EXPORT void get_vector(float* out);

POD_EXPORT void register_debug_callback(unity_debug_callback callback);

POD_EXPORT void POD_init();
POD_EXPORT void POD_update();
POD_EXPORT void POD_exit();
POD_EXPORT void POD_get_walk_linear_speed_vector(float* output);
POD_EXPORT long long POD_get_most_recent_time_code();
