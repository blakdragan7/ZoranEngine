#pragma once

// convenience defines

#define zEngine ZoranEngine::Instance()
#define pEngine zEngine->GetPhysicsEngine()
#define rEngine zEngine->GetRenderer()
#define r2Engine zEngine->GetRenderer2D()
#define r3Engine zEngine->GetRenderer3D()
#define aEngine zEngine->GetAudioEngine()
#define DebugWindow zEngine->GetDebugWindow()
#define Log(...) zEngine->logger->LogString(__VA_ARGS__);

#define LOG_ERROR zEngine->logger->WrapperForLevel(LogLevel_Error)
#define LOG_WARNING zEngine->logger->WrapperForLevel(LogLevel_Warning)
#define LOG_DEBUG zEngine->logger->WrapperForLevel(LogLevel_Debug)
#define LOG_INFO zEngine->logger->WrapperForLevel(LogLevel_Info)
#define LOG_VERBOSE zEngine->logger->WrapperForLevel(LogLevel_Verbose)

#include <Utils/LoggerBase.h>
#include <Core/WindowBase.h>
#include <Core/PlatformTypes.h>
#include <Core/CommomTypes.h>
#include <Math/Vector3.h>
#include <Math/Vector2.h>
#include <vector>
#include <Core/ZoranEngine.h>

enum SpawnCollisionBehaviour
{
	Spawn_Collision_Behaviour_Ignore_Collision, // spawn regardless of collision
	Spawn_Collision_Behaviour_Dont_Spawn, // don't sawn if colliding
	Spawn_Collision_Behaviour_Adjust, // try to adjust the position of spawned object before spawning
};

class ThreadBase;
class RenderEngineBase;
class RenderEngine2DBase;
class RenderEngine3DBase;
class PhysicsEngine;
class SceneObject;
class ITickableObject;
class PlayerInstanceBase;
class AllocatorBase;
class AudioEngineBase;
class ResourceManager;
class ZGIDebugWindow;
class ZGIVirtualWindow;
class ZGIGameVirtualWindow;
class ZoranEngine_EXPORT ZoranEngine
{
private:
	ZGIDebugWindow * debugWindow;
	ZGIGameVirtualWindow* gameWindow;
	WindowBase* mainWindow;
	RenderEngine2DBase* main2DRenderEngine;
	RenderEngine3DBase* main3DRenderEngine;
	PlayerInstanceBase* mainPlayer;

	static ZoranEngine* instance;
	bool shouldRun;
	bool isPaused;
	bool step;
	bool is3D;

	PhysicsEngine* physicsEngine;

	std::vector<ITickableObject*>* allTickables;
	std::vector<SceneObject*>* allSceneObjects;

	AudioEngineBase* audioEngine;

public:
	LoggerBase* logger;
	ResourceManager* rm;

	static bool canRenderDebug;

private:
	
	// add this scene object to the scene and updates it
	void AddSceneObject(SceneObject* object);
	// cleanup all invalid scene objects
	void CleanObjects();

public:
	ZoranEngine();
	~ZoranEngine();

	inline void StopRunning() { shouldRun = false; }

	int MainLoop();
	bool Init();
	void CreateGameModeWindows(bool is3D);
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
	// TODO: Eventually, these well be removed with a more custom way of choosing how the level is loaded
	void Setup2DScene(float centerx, float centery, float width, float height); // in meters
	void Setup2DScene(Vector2D center, Vector2D size); // in meters
	void Setup3DScene(Vector3D center, Vector3D size, float fov,float nearp,float farp); // in meters

	template<typename WindowClass>
	WindowClass* SetRootWindow()
	{
		if (mainWindow)
		{
			return mainWindow->SetRootVirtualWindow<WindowClass>(Vector2D(0,0), mainWindow->GetSize(), mainWindow->GetSize());
		}

		return nullptr;
	}

	void SetRootWindow(ZGIVirtualWindow* rootWindow);

	void DrawStep();

	template<class SceneObjectClass>
	SceneObjectClass* SpawnSceneObjectAtLocation(std::string name, Vec2D position, Vec2D size, SpawnCollisionBehaviour = Spawn_Collision_Behaviour_Dont_Spawn)
	{
		// TODO: collision test
		if (false)
		{
			Log(LogLevel_Info, "Spawn Scene Object found collision");
			return  nullptr;
		}

		SceneObjectClass* sceneObject = new SceneObjectClass(name);
		sceneObject->SetPosition(position);
		sceneObject->SetScale(size);
		sceneObject->PreCaclModel();

		AddSceneObject(sceneObject);

		return sceneObject;
	}

    template<class SceneObjectClass>
	SceneObjectClass* SpawnSceneObjectAtLocation(std::string name, Vec3D position, Vec3D size, SpawnCollisionBehaviour=Spawn_Collision_Behaviour_Dont_Spawn)
	{
		// TODO: collision test
		if (false)return  nullptr;

		SceneObjectClass* sceneObject = new SceneObjectClass(name);
		sceneObject->SetPosition(position);
		sceneObject->SetScale(size);
		sceneObject->PreCaclModel();
		AddSceneObject(sceneObject);

		return sceneObject;
	}

	// do not add scene objects or any subclass of scene objects this way, instead use SpawnSceneObject*
	// note the engine does not own tickable objects so it will expect them to be removed before they are destroyed
	void AddTickableObject(ITickableObject* object);
	// this does not destroy object
	void RemoveTickableObject(ITickableObject* object);

	// Scene Object MUST NEVER BE DELETED OUTSIDE OF THIS FUNCTION
	void DestroySceneObject(SceneObject* object);

	inline RenderEngineBase* GetRenderer()const;
	inline RenderEngine2DBase* GetRenderer2D()const { return main2DRenderEngine; }
	inline RenderEngine3DBase* GetRenderer3D()const { return main3DRenderEngine; }
	inline PhysicsEngine* GetPhysicsEngine()const { return physicsEngine; }
	inline AudioEngineBase* GetAudioEngine()const { return audioEngine; }
	inline WindowBase* GetMainWindow()const { return mainWindow; }

	inline ZGIDebugWindow* GetDebugWindow() { return debugWindow; };
	static inline ZoranEngine* Instance() { return instance; }

	inline void SetPaused(bool paused) { isPaused = paused; }

	const char* GetVersion()const;
	void GetVersion(unsigned &Major,unsigned &Minor,unsigned &Revision)const;
};
