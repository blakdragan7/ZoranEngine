#include "stdafx.h"
#include "ConsoleLogger.h"
#include <stdio.h>

#include <iostream>

ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::LogString(ELogLevel logLevel, const char * format, ...)
{
	if (this->logLevel > logLevel)return;
	WaitForMutex();
	va_list arg;

	va_start(arg, format);
	vfprintf(stdout, format, arg);
	fprintf(stdout, "\n");
	fflush(stdout);
	va_end(arg);
	m->unlock();
}

void ConsoleLogger::WriteString(std::string string)
{
	WaitForMutex();
	std::cout << string;
	m->unlock();
}
