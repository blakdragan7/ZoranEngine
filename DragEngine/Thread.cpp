#include "stdafx.h"
#include "Thread.h"


Thread::Thread()
{
}


Thread::~Thread()
{
}

void Thread::Sleep(float seconds)
{
#ifdef _WIN32
	::Sleep(seconds * 1000.0F);
#else
#endif
}
