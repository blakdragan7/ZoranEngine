#pragma once

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE WindowHandle;
#ifdef BUILD_DLL
#define ZoranEngine_EXPORT __declspec(dllexport)
#else
#define ZoranEngine_EXPORT __declspec(dllimport)
#endif
#else
#define ZoranEngine_EXPORT
#endif
