#pragma once
#include "Utils/LoggerBase.h"
#include "PlatformTypes.h"
#include "CommomTypes.h"
#include "Math/Vector3.h"
#include <vector>

class ThreadBase;
class WindowBase;
class RenderEngineBase;
class PhysicsEngine;
class SceneObject;
class TickableObject;
class CameraBase;
class ZoranEngine_EXPORT ZoranEngine
{
private:
	WindowBase* mainWindow;
	RenderEngineBase* mainRenderEngine;

	CameraBase* camera;

	static ZoranEngine* instance;
	bool shouldRun;
	bool isPaused;
	bool step;

	PhysicsEngine* physicsEngine;
#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<TickableObject*> allTickables;
	std::vector<SceneObject*> allSceneObjects;
#pragma warning(pop)

public:
	ZoranEngine();
	~ZoranEngine();

	LoggerBase* logger;

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

	void Setup2DScene(float centerx, float centery, float width, float height); // in meters
	void Setup2DScene(Vector2D center, Vector2D size); // in meters
	void SetupScene(float centerx, float centery, float width, float height, float depth); // in meters
	void SetupScene(Vector3D center, Vector3D size); // in meters

	inline CameraBase* GetCamera() { return camera; }
	// do not add scene objects or any subclass of scene objects this way, instead use AddSceneObject
	void AddTickableObject(TickableObject* object);
	// add this scene object to the scene and updates it
	void AddSceneObject(SceneObject* object);
	// Scene Object MUST NEVER BE DELETED OUTSIDE OF THIS FUNCTION
	void DestroySceneObject(SceneObject* object);
	void RemoveTickableObject(TickableObject* object);

	inline RenderEngineBase* GetRenderer() { return mainRenderEngine; }
	inline PhysicsEngine* GetPhysicsEngine() { return physicsEngine; }

	static inline ZoranEngine* Instance() { return instance; }

	void SetPaused(bool paused) { isPaused = paused; }

	const char* GetVersion();
	void GetVersion(unsigned &Major,unsigned &Minor,unsigned &Revision);

	// Creates a thread for the current platform and returns an instance to it.
	// You must release this instance when it is done.
	ThreadBase* CreateThread();
};

// convenience defines

#define zEngine ZoranEngine::Instance()
#define pEngine zEngine->GetPhysicsEngine()
#define rEngine zEngine->GetRenderer()
#define Log(...) ZoranEngine::Instance()->logger->LogString(__VA_ARGS__);
