#include "stdafx.h"

#include <Utils/Random.h>
#include <Core/ZoranEngine.h>
#include <Core/WindowBase.h>
#include <Core/TickableObject.h>
#include <Core/2D/OrthoCamera.h>
#include <Physics/PhysicsEngine.h>
#include <Windows/WindowsWindow.h>
#include <Utils/HighPrecisionClock.h>
#include <OpenGL/OpenGLRenderEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/Collision/CollisionBucketBase.h>

#include <Core/Allocators/CAllocator.h>

#include <iostream>

#include "SceneObject.h"

#include "Utils/VectorAddons.hpp"

#include "Version.h"
#include "Utils/ConsoleLogger.h"
#ifdef _WIN32
#include "Windows/WindowsThread.h"
#else

#endif

ZoranEngine* ZoranEngine::instance = 0;

ZoranEngine::ZoranEngine()
{
	mainWindow = 0;
	if (instance)throw std::exception("There can only be one ZoranEngine instance !");
	instance = this;
	shouldRun = true;
	physicsEngine = new PhysicsEngine();
	mainRenderEngine = 0;
	isPaused = false;
	logger = new ConsoleLogger();
	logger->SetLogLevel(LogLevel_Default);
	step = false;
	camera = 0;

	defaultAllocator = new CAllocator();

	allSceneObjects = new std::vector<SceneObject*>(8);
	allTickables = new std::vector<TickableObject*>(8);
}

ZoranEngine::~ZoranEngine()
{
	for (auto object : *allSceneObjects)
	{
		object->Destroy();
	}

	delete allSceneObjects;
	delete allTickables;

	if (mainWindow)delete mainWindow;
	if (physicsEngine)delete physicsEngine;
	if (mainRenderEngine)delete mainRenderEngine;

	delete defaultAllocator;

	delete logger;
}

int ZoranEngine::MainLoop()
{
	// Each Os will have it's own main loop
	// should probably move these to there own functions
#ifdef _WIN32
	MSG       msg = { 0 };

	HighPrecisionClock clock;
	HighPrecisionClock statisticsClock;
	double statistics = 0;

	while (WM_QUIT != msg.message && shouldRun)
	{
		double FPSTime = clock.GetDiffSeconds();
		float deltaTime = (1.0f / 60.0f);
		clock.TakeClock();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (deltaTime == 0)deltaTime = 0;

		if (isPaused == false || step == true)
		{
			if (step)
			{
				step = false;
				//deltaTime = 0.033; 
			}
			statisticsClock.TakeClock();
			if (physicsEngine)physicsEngine->UpdateAll(deltaTime);

			statistics = statisticsClock.GetDiffSeconds();
			Log(LogLevel_Verbose, "physicsEngine->UpdateAll() took %f ms\n", statistics*1000);

			statisticsClock.TakeClock();
			for (auto object : *allTickables)
			{
				object->Tick(deltaTime);
			}

			statistics = statisticsClock.GetDiffSeconds();
			Log(LogLevel_Verbose, "All object->Tick()'s took %f ms\n", statistics*1000);
		}
		else
		{

		}

		statisticsClock.TakeClock();
		if (mainWindow)mainWindow->MainDraw();
		statistics = statisticsClock.GetDiffSeconds();
		Log(LogLevel_Verbose, "mainWindow->MainDraw() took %f ms\n", statistics * 1000);
	}
#endif

	return 0;
}

bool ZoranEngine::Init()
{
	Random::Init();
	
	mainRenderEngine = new OpenGLRenderEngine();
	WindowsWindow* window = new WindowsWindow(this);
	window->MakeWindow("test", 0, 0, 1920, 1080);
	mainRenderEngine->InitEngine(window->GetHandle());

	mainWindow = window;

	return true;
}

void ZoranEngine::Setup2DScene(float centerx, float centery, float width, float height)
{
	physicsEngine->SetupFor2D(Vec2D(centerx, centery), Vec2D(width, height));
	camera = new OrthoCamera("camera", width, height, 0);
	camera->Translate(centerx, centery, 0);
	camera->ScreenResized(mainWindow->GetSize());
}

void ZoranEngine::Setup2DScene(Vector2D center, Vector2D size)
{
	physicsEngine->SetupFor2D(center, size);
	camera = new OrthoCamera("camera",size.x,size.y, 0);
	camera->Translate(center.x, center.y, 0);
}

void ZoranEngine::SetupScene(float centerx, float centery, float width, float height, float depth)
{
	throw std::exception("SetupScene Not Implemented yet !");
}

void ZoranEngine::SetupScene(Vector3D center, Vector3D size)
{
	throw std::exception("SetupScene Not Implemented yet !");
}

void ZoranEngine::KeyEvent(KeyEventType type, unsigned key)
{
	switch (type)
	{
	case KEY_DOWN:
		switch (key)
		{
			case VK_SPACE:
			{
				isPaused = !isPaused;
				break;
			}
			case VK_ESCAPE:
				shouldRun = false;
				break;
			case 'P':
				if (physicsEngine)physicsEngine->GetCollisionBucketRoot()->PrintAllContents();
				if (physicsEngine)physicsEngine->GetCollisionBucketRoot()->PrintAllCollisions();
				break;
			case 'S':
				if (isPaused)step = true;
				break;
		}
		break;
	case KEY_UP:
		if (key == Key_F11)mainWindow->SetWindowFullScreen(!mainWindow->IsFullScreen());
		break;
	}
}

void ZoranEngine::MouseEvent(MouseEventType, float value)
{
}

void ZoranEngine::MouseMove(float x, float y)
{
}

void ZoranEngine::ScreenResized(float width, float height)
{
	if(camera)camera->ScreenResized(width, height);
}

void ZoranEngine::AddTickableObject(TickableObject * object)
{
	allTickables->push_back(object);
}

void ZoranEngine::AddSceneObject(SceneObject * object)
{
	if (object->GetCollision())object->GetCollision()->SetBoundsBySceneObject();
	if(object->willEverTick)AddTickableObject((TickableObject*)(object));
	mainRenderEngine->AddSceneObject(object);
	if(object->GetPhysics())
		physicsEngine->AddPhysicsObject(object->GetPhysics());
	CollisionObjectBase* collision = object->GetCollision();
	if (collision)GetPhysicsEngine()->AddCollisionObject(collision);
}

void ZoranEngine::DestroySceneObject(SceneObject * object)
{
	remove(*allSceneObjects, object);
	mainRenderEngine->RemoveSceneObject(object);
	if (object->GetPhysics())delete object->GetPhysics();
	delete object;
}

void ZoranEngine::RemoveTickableObject(TickableObject * object)
{
	remove(*allTickables, object);
}

const char * ZoranEngine::GetVersion()
{
	return Version;
}

void ZoranEngine::GetVersion(unsigned &Major, unsigned &Minor, unsigned &Revision)
{
	Major = VERSION_MAJOR;
	Minor = VERSION_MINOR;
	Revision = VERSION_REVISION;
}

ThreadBase * ZoranEngine::CreateThread()
{
	ThreadBase* thread = 0;
#ifdef _WIN32
	thread = new WindowsThread();
#else
	throw std::exception("Threads Not Implented For This Platform !");
#endif
	return thread;
}
