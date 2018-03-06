#pragma once

#ifdef _WIN32
#ifdef DLL_EXPORT
#define POD_EXPORT __declspec(dllexport)
#else
#define POD_EXPORT __declspec(dllimport)
#endif
#else
#define POD_EXPORT
#endif
