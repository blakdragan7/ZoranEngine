#pragma once
#include "PlatformTypes.h"
#include "CommomTypes.h"
#include <vector>

class ThreadBase;
class WindowBase;
class RenderEngineBase;
class PhysicsEngine;
class SceneObject;
class TickableObject;
class DRAGENGINE_EXPORT DragEngine
{
private:
	WindowBase* mainWindow;
	RenderEngineBase* mainRenderEngine;

	static DragEngine* instance;
	bool shouldRun;

	PhysicsEngine* physicsEngine;
#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<TickableObject*> allTickables;
	std::vector<SceneObject*> allSceneObjects;
#pragma warning(pop)

public:
	DragEngine();
	~DragEngine();

	inline void StopRunning() { shouldRun = false; }

	int MainLoop();
	bool Init();
	/* 
	* All Input Events are converted to a common format, normall 'A' on OSx is not the same as 'A' on Windows
	* so we convert them first in there respective WindowBase Implementations therefore, 
	* to check for a key you must only do a simple comparison such as key == 'A' once inside KeyEvent
	* currently this does not support case sensitive because 'a' == 'A' That may or may not need to be changed later
	*/
	void KeyEvent(KeyEventType type,unsigned key);
	void MouseEvent(MouseEventType,float value);
	void MouseMove(float x,float y);

	// do not add scene objects or any subclass of scene objects this way, instead use AddSceneObject
	void AddTickableObject(TickableObject* object);
	// add this scene object to the scene and updates it
	void AddSceneObject(SceneObject* object);
	// Scene Object MUST NEVER BE DELETED OUTSIDE OF THIS FUNCTION
	void DestroySceneObject(SceneObject* object);
	void RemoveTickableObject(TickableObject* object);

	inline RenderEngineBase* GetRenderer() { return mainRenderEngine; }
	inline PhysicsEngine* GetPhysicsEngine() { return physicsEngine; }

	static inline DragEngine* Instance() { return instance; }

	const char* GetVersion();
	void GetVersion(unsigned &Major,unsigned &Minor,unsigned &Revision);

	// Creates a thread for the current platform and returns an instance to it.
	// You must release this instance when it is done.
	ThreadBase* CreateThread();
};

// convenience defines

#define dEngine DragEngine::Instance()
#define pEngine DragEngine::Instance()->GetPhysicsEngine()
#define rEngine DragEngine::Instance()->GetRenderer()

