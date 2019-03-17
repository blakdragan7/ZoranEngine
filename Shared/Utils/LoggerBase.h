#pragma once
#include "PlatoformTypes.h"

#include <sstream>
#include <mutex>

enum ELogLevel {
	LogLevel_Verbose = 0,
	LogLevel_Info = 1,
	LogLevel_Debug = 2,
	LogLevel_Warning = 3,
	LogLevel_Error = 4,
	LogLevel_None = 5
};
class LogWrapper;
class DLL_EXPORT LoggerBase
{
protected:
	ELogLevel logLevel;
	std::mutex* m;

protected:
	void WaitForMutex();

public:
	LoggerBase();
	virtual ~LoggerBase();

	LogWrapper WrapperForLevel(ELogLevel logLevel);

	void SetLogLevel(ELogLevel logLevel);
	virtual void LogString(ELogLevel logLevel, const char* log, ...) = 0;
	virtual void WriteString(std::string string) = 0;

};

class LogWrapper
{
private:
	std::stringstream ss;
	bool doThings;

	LoggerBase* logger;

public:
	LogWrapper(LoggerBase* logger, bool doThings) : logger(logger), doThings(doThings) {}
	LogWrapper(const LogWrapper& other) : doThings(other.doThings)
	{
		ss << other.ss.rdbuf();
		logger = other.logger;
	}
	~LogWrapper()
	{
		if (doThings)
		{
			logger->WriteString(ss.str());
		}
	}

	template <typename t>
	LogWrapper& operator <<(const t& l)
	{
		if (doThings)ss << l;
		return *this;
	}

	LogWrapper& operator<<(std::ostream&(*f)(std::ostream&))
	{
		f(ss);
		return *this;
	}
};
