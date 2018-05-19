#include "stdafx.h"
#include "WindowsThread.h"
#include <iostream>
#include <Windows.h>

WindowsThread::WindowsThread() : ThreadBase()
{
	threadID = 0;
	threadHandle = 0;
}


WindowsThread::~WindowsThread()
{
	if(threadHandle)CloseHandle(threadHandle);
}

void WindowsThread::StartThread()
{
	ThreadBase::StartThread();
	threadHandle = CreateThread(NULL, 0, WindowsThreadFunc, this, 0, &threadID);
	if (threadHandle == NULL)
	{
		// maybe through exception here, not sure yet
		std::cerr << "WindowsThread::StartThread() Could Not Creat Thread !\n";
	}
}

void WindowsThread::StopThread()
{
	ThreadBase::StopThread();
	// block for thread stopping
	WaitForSingleObject(threadHandle,INFINITE);

	threadID = 0;
	threadHandle = 0;
}

DWORD WINAPI WindowsThreadFunc(LPVOID lpParam)
{
	WindowsThread* thread = static_cast<WindowsThread*>(lpParam);

	if (thread != 0)
	{
		while (thread->GetIsRunning())
		{
			thread->Run(0);
		}
	}
}