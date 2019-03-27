#include "stdafx.h"
#include "LoggerBase.h"

#include <ostream>

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

