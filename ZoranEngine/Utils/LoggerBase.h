#pragma once
enum ELogLevel {
	LogLevel_Verbose = 0,
	LogLevel_Info = 1,
	LogLevel_Debug = 2,
	LogLevel_Warning = 3,
	LogLevel_Error = 4,
	LogLevel_None = 5
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

