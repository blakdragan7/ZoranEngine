#pragma once

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE WindowHandle;
#ifdef BUILD_DLL
#define DRAGENGINE_EXPORT __declspec(dllexport)
#else
#define DRAGENGINE_EXPORT __declspec(dllimport)
#endif
#else
#define DRAGENGINE_EXPORT
#endif
