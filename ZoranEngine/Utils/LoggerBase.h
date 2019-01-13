#pragma once
enum ELogLevel {
	LogLevel_Verbose = 0,
	LogLevel_Debug = 1,
	LogLevel_Warning = 2,
	LogLevel_Error = 3,
	LogLevel_None = 4
};

class LoggerBase
{
protected:
	ELogLevel logLevel;

public:
	LoggerBase();
	virtual ~LoggerBase();

	void SetLogLevel(ELogLevel logLevel);
	virtual void LogString(ELogLevel logLevel,const char* log,...) = 0;
};

