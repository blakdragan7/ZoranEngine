#pragma once
#include "Core/ThreadBase.h"
class ZoranEngine_EXPORT WindowsThread : public ThreadBase
{
private:
	DWORD threadID;
	HANDLE threadHandle;

public:
	WindowsThread();
	~WindowsThread();

	void StartThread()override;
	void StopThread()override;

	friend DWORD WINAPI WindowsThreadFunc(LPVOID lpParam);
};

