#pragma once
#include "LoggerBase.h"
class ConsoleLogger : public LoggerBase
{
public:
	ConsoleLogger();
	~ConsoleLogger();

	virtual void LogString(ELogLevel logLevel, const char* format,...)override;
};

