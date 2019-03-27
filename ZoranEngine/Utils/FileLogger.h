#pragma once
#include "LoggerBase.h"
#include <Core/PlatformTypes.h>

#include <fstream>

class ZoranEngine_EXPORT FileLogger : public LoggerBase
{
	std::fstream* file_;
	std::string* path;
	bool isGood;

public:
	FileLogger(std::string filePath);
	~FileLogger();

	virtual void LogString(ELogLevel logLevel, const char* format, ...)override;
	virtual void WriteString(std::string string)override;
	inline bool IsGood()const { return isGood; }
};

