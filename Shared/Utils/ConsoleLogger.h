#pragma once
#include "LoggerBase.h"
#include "PlatoformTypes.h"
class DLL_EXPORT ConsoleLogger : public LoggerBase
{
public:
	ConsoleLogger();
	~ConsoleLogger();

	virtual void LogString(ELogLevel logLevel, const char* format,...)override;
	virtual void WriteString(std::string string)override;
};

