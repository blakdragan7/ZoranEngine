#pragma once
enum ELogLevel {
	LogLevel_Verbose = 0,
	LogLevel_Debug = 1,
	LogLevel_Default = 2,
	LogLevel_None = 3
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

