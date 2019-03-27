#include "stdafx.h"
#include "FileLogger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define file (*file_)

FileLogger::FileLogger(std::string filePath)
{
	path = new std::string(filePath);
	file_ = new std::fstream;
	file_->open(filePath, std::ios::out);
	if (file_->is_open() == false)
	{
		std::cout << "Could Not Open File For Logging! File " << filePath << std::endl;
		isGood = false;
	}
	else
	{
		isGood = true;
	}
}

FileLogger::~FileLogger()
{
	if (file_)
	{
		file_->close();
		delete file_;
	}

	delete path;
}

void FileLogger::LogString(ELogLevel logLevel, const char * format, ...)
{
	if (isGood)
	{
		WaitForMutex();

		if (this->logLevel > logLevel)return;

		// initialize use of the variable argument array
		va_list vaArgs;
		va_start(vaArgs, format);

		// reliably acquire the size from a copy of
		// the variable argument array
		// and a functionally reliable call
		// to mock the formatting
		va_list vaCopy;
		va_copy(vaCopy, vaArgs);
		const int iLen = std::vsnprintf(NULL, 0, format, vaCopy);
		va_end(vaCopy);

		// return a formatted string without
		// risking memory mismanagement
		// and without assuming any compiler
		// or platform specific behavior
		std::vector<char> zc(iLen + 1);
		std::vsnprintf(zc.data(), zc.size(), format, vaArgs);
		va_end(vaArgs);
		std::string formatedString(zc.data(), zc.size());

		file_->write(formatedString.c_str(),formatedString.size());

		m->unlock();
	}
	else
	{
		std::cout << "Error: Trying To Write To Log File when it wasn't opened !\n";
	}
}

void FileLogger::WriteString(std::string string)
{
	if (isGood)
	{
		WaitForMutex();
		file_->write(string.c_str(), string.size());
		m->unlock();
	}
	else
	{
		std::cout << "Error: Trying To Write To Log File when it wasn't opened !\n";
	}
}
