#include "stdafx.h"
#include "ThreadBase.h"
#ifdef _WIN32
#include <Windows/WindowsThread.h>
#endif

#include <assert.h>

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

ThreadBase * ThreadBase::CreatePlatformThread()
{
	ThreadBase* thread = 0;
#ifdef _WIN32
	thread = new WindowsThread();
#else
	Log(LogLevel_Error,"There is No Thread Implementation For this Platform !!");
	assert(false);
#endif
	return thread;
}