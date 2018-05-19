#pragma once
#include "PlatformTypes.h"

/*
	ThreadBase is created via DragEngine::CreateThread(). 
	DragEngine will choose the correct implementation. 
	Set the RunnableFun to execute code on a background thread
*/

typedef void (*RunnableFunc)(void*);
class DRAGENGINE_EXPORT ThreadBase
{
private:
	RunnableFunc func;
	bool hasFunc;
	bool isRunning;

protected:
	void RunFnction(void* data);

public:
	ThreadBase();
	virtual ~ThreadBase();

	// data is currently unused but is here for future use or for implementation in a subclass
	virtual void Run(void* data);
	// static function that sleeps the current thread platform specifically
	static void Sleep(float seconds);

	inline bool GetIsRunning() { return isRunning; };
	inline void SetFunc(RunnableFunc func) { this->func = func; };

	virtual void StartThread(); // starts the thread
	virtual void StopThread(); // blocks until thread is stopped
};
