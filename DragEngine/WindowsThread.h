#pragma once
#include "ThreadBase.h"
class DRAGENGINE_EXPORT WindowsThread : public ThreadBase
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

