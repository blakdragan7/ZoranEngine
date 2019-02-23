#pragma once

#include <Utils/LoggerBase.h>

extern LoggerBase* logger;

#define LOG(...) logger->LogString(__VA_ARGS__)

#define LOG_ERROR logger->WrapperForLevel(LogLevel_Error)
#define LOG_WARNING logger->WrapperForLevel(LogLevel_Warning)
#define LOG_DEBUG logger->WrapperForLevel(LogLevel_Debug)
#define LOG_INFO logger->WrapperForLevel(LogLevel_Info)
#define LOG_VERBOSE logger->WrapperForLevel(LogLevel_Verbose)
#define LOG_ALL logger->WrapperForLevel(LogLevel_None)