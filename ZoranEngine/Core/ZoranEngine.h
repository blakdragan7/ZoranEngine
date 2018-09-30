#pragma once
#include "Utils/LoggerBase.h"
#include "PlatformTypes.h"
#include "CommomTypes.h"
#include "Math/Vector3.h"
#include <vector>

class ThreadBase;
class WindowBase;
class RenderEngineBase;
class RenderEngine2DBase;
class RenderEngine3DBase;
class PhysicsEngine;
class SceneObject;
class TickableObject;
class CameraBase;
class AllocatorBase;
class AudioEngineBase;
class TriangleStripRenderer;
class ShaderProgramBase;

class ZoranEngine_EXPORT ZoranEngine
{
private:
	WindowBase* mainWindow;
	RenderEngine2DBase* main2DRenderEngine;
	RenderEngine3DBase* main3DRenderEngine;
	TriangleStripRenderer* fullScreenRenderer;
	ShaderProgramBase* fullScreenProgram;

	CameraBase* camera;

	static ZoranEngine* instance;
	bool shouldRun;
	bool isPaused;
	bool step;
	bool is3D;

	PhysicsEngine* physicsEngine;

	std::vector<TickableObject*>* allTickables;
	std::vector<SceneObject*>* allSceneObjects;

	AudioEngineBase* audioEngine;

public:
	AllocatorBase * defaultAllocator;

	static bool canRenderDebug;

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
	void ScreenResized(float width, float height);

	void Setup2DScene(float centerx, float centery, float width, float height); // in meters
	void Setup2DScene(Vector2D center, Vector2D size); // in meters
	void Setup3DScene(Vector3D center, Vector3D size, float fov,float nearp,float farp); // in meters

	void DrawStep();

	inline CameraBase* GetCamera() { return camera; }
	// do not add scene objects or any subclass of scene objects this way, instead use AddSceneObject
	void AddTickableObject(TickableObject* object);
	// add this scene object to the scene and updates it
	void AddSceneObject(SceneObject* object);
	// Scene Object MUST NEVER BE DELETED OUTSIDE OF THIS FUNCTION
	void DestroySceneObject(SceneObject* object);
	void RemoveTickableObject(TickableObject* object);

	inline RenderEngineBase* GetRenderer()const;
	inline RenderEngine2DBase* GetRenderer2D()const { return main2DRenderEngine; }
	inline RenderEngine3DBase* GetRenderer3D()const { return main3DRenderEngine; }
	inline PhysicsEngine* GetPhysicsEngine()const { return physicsEngine; }
	inline AudioEngineBase* GetAudioEngine()const { return audioEngine; }

	static inline ZoranEngine* Instance() { return instance; }

	inline void SetPaused(bool paused) { isPaused = paused; }

	const char* GetVersion()const;
	void GetVersion(unsigned &Major,unsigned &Minor,unsigned &Revision)const;
};

// convenience defines

#define zEngine ZoranEngine::Instance()
#define pEngine zEngine->GetPhysicsEngine()
#define rEngine zEngine->GetRenderer()
#define r2Engine zEngine->GetRenderer2D()
#define r3Engine zEngine->GetRenderer3D()
#define aEngine zEngine->GetAudioEngine()
#define Log(...) ZoranEngine::Instance()->logger->LogString(__VA_ARGS__);
