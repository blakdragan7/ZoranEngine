#pragma once

#ifdef _WIN32
#include <Windows.h>
typedef HANDLE WindowHandle;
#define DRAGENGINE_EXPORT __declspec(dllexport)
#endif
