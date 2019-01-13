#pragma once
#include <Utils\LoggerBase.h>

class FileLogger : public LoggerBase
{
	std::fstream* file_;
	std::string path;
	bool isGood;

public:
	FileLogger(std::string filePath);
	~FileLogger();

	virtual void LogString(ELogLevel logLevel, const char* format, ...)override;
	inline bool IsGood()const { return isGood; }
};

