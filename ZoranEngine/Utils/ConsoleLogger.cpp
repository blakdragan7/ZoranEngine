#include "stdafx.h"
#include "ConsoleLogger.h"
#include <stdio.h>

ConsoleLogger::ConsoleLogger()
{
}


ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::LogString(ELogLevel logLevel, const char * format, ...)
{
	if (this->logLevel > logLevel)return;
	va_list arg;

	va_start(arg, format);
	vfprintf(stdout, format, arg);
	fflush(stdout);
	va_end(arg);

}
