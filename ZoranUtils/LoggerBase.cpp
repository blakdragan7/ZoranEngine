#include "stdafx.h"
#include "LoggerBase.h"


LoggerBase::LoggerBase() : logLevel(LogLevel_Warning)
{
}


LoggerBase::~LoggerBase()
{
}

void LoggerBase::SetLogLevel(ELogLevel logLevel)
{
	this->logLevel = logLevel;
}
