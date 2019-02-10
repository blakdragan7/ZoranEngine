#pragma once

#ifdef _WIN32 // if windows

#ifdef EXPORT // if export defined
#define DLL_EXPORT __declspec(dllexport)
#else // otherwise
#define DLL_EXPORT __declspec(dllimport)
#endif // end if export

#else // not windows
#define DLL_EXPORT
#endif // end if windows