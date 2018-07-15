#include "stdafx.h"

#include "Utils/Random.h"
#include "ZoranEngine.h"
#include "WindowBase.h"
#include "TickableObject.h"
#include "Physics/PhysicsEngine.h"
#include "Windows/WindowsWindow.h"
#include "OpenGL/OpenGLRenderEngine.h"
#include "Utils/HighPrecisionClock.h"
#include "Physics/Collision/CollisionObjectBase.h"
#include "Physics/Collision/CollisionBucketBase.h"
#include <Core/2D/OrthoCamera.h>
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
	logger->SetLogLevel(LogLevel_Debug);
	step = false;
}

ZoranEngine::~ZoranEngine()
{
	for (auto object : allSceneObjects)
	{
		object->Destroy();
	}

	if (mainWindow)delete mainWindow;
	if (physicsEngine)delete physicsEngine;
	if (mainRenderEngine)delete mainRenderEngine;

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
	float statistics = 0;

	while (WM_QUIT != msg.message && shouldRun)
	{
		float deltaTime = clock.GetDiffSeconds();
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
				deltaTime = 0.033;
			}
			statisticsClock.TakeClock();
			if (physicsEngine)physicsEngine->UpdateAll(deltaTime);

			statistics = statisticsClock.GetDiffSeconds();
			Log(LogLevel_Verbose, "physicsEngine->UpdateAll() took %f ms\n", statistics*1000);

			statisticsClock.TakeClock();
			for (auto object : allTickables)
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

		//std::cout << "fps " << 1.0 / deltaTime << std::endl;
	}
#endif

	return 0;
}

bool ZoranEngine::Init()
{
	Random::Init();
	
	mainRenderEngine = new OpenGLRenderEngine();
	WindowsWindow* window = new WindowsWindow(mainRenderEngine);
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

void ZoranEngine::AddTickableObject(TickableObject * object)
{
	allTickables.push_back(object);
}

void ZoranEngine::AddSceneObject(SceneObject * object)
{
	if (object->GetCollision())object->GetCollision()->SetBoundsBySceneObject();
	if(object->willEverTick)AddTickableObject((TickableObject*)(object));
	mainRenderEngine->AddSceneObject(object);
	physicsEngine->AddPhysicsObject(object->GetPhysics());
	CollisionObjectBase* collision = object->GetCollision();
	if (collision)GetPhysicsEngine()->AddCollisionObject(collision);
}

void ZoranEngine::DestroySceneObject(SceneObject * object)
{
	mainRenderEngine->RemoveSceneObject(object);
	delete object;
	delete object->GetPhysics();
}

void ZoranEngine::RemoveTickableObject(TickableObject * object)
{
	remove(allTickables,object);
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
