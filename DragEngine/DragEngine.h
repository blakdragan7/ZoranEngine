#pragma once
#include "PlatformTypes.h"
#include "CommomTypes.h"

class ThreadBase;
class WindowBase;
class RenderEngineBase;
class DRAGENGINE_EXPORT DragEngine
{
private:
	WindowBase* mainWindow;
	RenderEngineBase* mainRenderEngine;

	static DragEngine* instance;
	bool shouldRun;

public:
	DragEngine();
	~DragEngine();

	inline void StopRunning() { shouldRun = false; }

	int MainLoop();
	bool Init();
	/* 
	* All Input Events are converted to a common format, normall 'A' on OSx is not the same as 'A' 
	* on Windows so we convert them first in there respective WindowBase Implementations
	* therefore, to check for a key you must only do a simple comparison such as key == 'A' once inside KeyEvent
	* currently this does not support case sensitive because 'a' == 'A' That may or may not need to be changed later
	*/
	void KeyEvent(KeyEventType type,unsigned key);
	void MouseEvent(MouseEventType,float value);
	void MouseMove(float x,float y);

	inline RenderEngineBase* GetRenderer() { return mainRenderEngine; }

	static inline DragEngine* Instance() { return instance; }

	// Creates a thread for the current platform and returns an instance to it.
	// You must release this instance when it is done.
	ThreadBase* CreateThread();
};

#define dEngine DragEngine::Instance()

