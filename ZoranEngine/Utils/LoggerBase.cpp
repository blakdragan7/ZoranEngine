#include "stdafx.h"
#include "LoggerBase.h"


LoggerBase::LoggerBase()
{
}


LoggerBase::~LoggerBase()
{
}

void LoggerBase::SetLogLevel(ELogLevel logLevel)
{
	this->logLevel = logLevel;
}
