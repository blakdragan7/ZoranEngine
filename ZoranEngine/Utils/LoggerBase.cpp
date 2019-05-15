#include "stdafx.h"
#include "LoggerBase.h"

#include <algorithm>
#include <ostream>
#include <string>

void LoggerBase::WaitForMutex()
{
#ifdef _WIN32
#include <Windows.h>
	while(m->try_lock() == false) Sleep(10);
#else
#endif
}

LoggerBase::LoggerBase() : logLevel(LogLevel_Warning)
{
	m = new std::mutex;
}

LoggerBase::~LoggerBase()
{
	delete m;
}

LogWrapper LoggerBase::WrapperForLevel(ELogLevel logLevel)
{
	return LogWrapper(this, this->logLevel <= logLevel);
}

void LoggerBase::SetLogLevel(ELogLevel logLevel)
{
	this->logLevel = logLevel;
}

bool LoggerBase::SetLogLevel(const std::string& logLevel)
{
	return LogLevelFromString(logLevel, this->logLevel);
}

bool LogLevelFromString(const std::string& string, ELogLevel& logLevel)
{
	std::string lower;
	std::transform(string.begin(), string.end(), lower.begin(), std::tolower);

	if (lower == "none")
	{
		logLevel = LogLevel_None;
		return true;
	}
	else if (lower == "error")
	{
		logLevel = LogLevel_Error;
		return true;
	}
	else if (lower == "debug")
	{
		logLevel = LogLevel_Debug;
		return true;
	}
	else if (lower == "warning")
	{
		logLevel = LogLevel_Warning;
		return true;
	}
	else if (lower == "info")
	{
		logLevel = LogLevel_Info;
		return true;
	}
	else if (lower == "verbose")
	{
		logLevel = LogLevel_Verbose;
		return true;
	}

	return false;
}