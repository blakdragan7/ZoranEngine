#include "stdafx.h"
#include "ThreadBase.h"


void ThreadBase::RunFnction(void * data)
{
	if (func == 0 || hasFunc == false)throw std::exception("RunFnction Called Wit Null Func !");
	func(data); 
}

ThreadBase::ThreadBase()
{
	hasFunc = false;
	func = 0;
}


ThreadBase::~ThreadBase()
{
}

void ThreadBase::Sleep(float seconds)
{
#ifdef _WIN32
	::Sleep((DWORD)(seconds * 1000.0F));
#else
#endif
}

void ThreadBase::StartThread()
{
	isRunning = true;
}

void ThreadBase::StopThread()
{
	isRunning = false;
}

void ThreadBase::Run(void * data)
{
	if(hasFunc)func(data);
}
