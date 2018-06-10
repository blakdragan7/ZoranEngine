#include "stdafx.h"

#include "Random.h"
#include "DragEngine.h"
#include "WindowBase.h"
#include "TickableObject.h"
#include "PhysicsEngine.h"
#include "WindowsWindow.h"
#include "OpenGLRenderEngine.h"
#include "HighPrecisionClock.h"
#include "CollisionObjectBase.h"
#include "CollisionBucketBase.h"
#include "OrthoCamera.h"
#include <iostream>

#include "SceneObject.h"

#include "VectorAddons.hpp"

#include "Version.h"
#include "ConsoleLogger.h"
#ifdef _WIN32
#include "WindowsThread.h"
#else

#endif

DragEngine* DragEngine::instance = 0;

DragEngine::DragEngine()
{
	mainWindow = 0;
	if (instance)throw std::exception("There can only be one DragEngine instance !");
	instance = this;
	shouldRun = true;
	physicsEngine = new PhysicsEngine();
	mainRenderEngine = 0;
	isPaused = false;
	logger = new ConsoleLogger();
	logger->SetLogLevel(LogLevel_Default);
}

DragEngine::~DragEngine()
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

int DragEngine::MainLoop()
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
		double deltaTime = clock.GetDiffSeconds();
		clock.TakeClock();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		statisticsClock.TakeClock();
		if (mainWindow)mainWindow->MainDraw();
		statistics = statisticsClock.GetDiffSeconds();
		Log(LogLevel_Verbose,"mainWindow->MainDraw() took %f ms\n", statistics*1000);
		if (isPaused == false)
		{
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
		std::cout << "fps " << 1.0 / deltaTime << std::endl;
	}
#endif

	return 0;
}

bool DragEngine::Init()
{
	Random::Init();
	
	mainRenderEngine = new OpenGLRenderEngine();
	WindowsWindow* window = new WindowsWindow(mainRenderEngine);
	window->MakeWindow("test", 0, 0, 1920, 1080);
	mainRenderEngine->InitEngine(window->GetHandle());

	mainWindow = window;

	return true;
}

void DragEngine::Setup2DScene(double centerx, double centery, double width, double height)
{
	physicsEngine->SetupFor2D(Vec3D(centerx, centery, 0), Vec3D(width, height, 10));
	camera = new OrthoCamera("camera", width, height, 0);
	camera->Translate(centerx, centery, 0);
}

void DragEngine::Setup2DScene(Vector3D center, Vector3D size)
{
	physicsEngine->SetupFor2D(center, size);
	camera = new OrthoCamera("camera",size.x,size.y, 0);
	camera->Translate(center.x, center.y, 0);
}

void DragEngine::SetupScene(double centerx, double centery, double width, double height, double depth)
{
	throw std::exception("SetupScene Not Implemented yet !");
}

void DragEngine::SetupScene(Vector3D center, Vector3D size)
{
	throw std::exception("SetupScene Not Implemented yet !");
}

void DragEngine::KeyEvent(KeyEventType type, unsigned key)
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
		}
		break;
	case KEY_UP:
		if (key == Key_F11)mainWindow->SetWindowFullScreen(!mainWindow->IsFullScreen());
		break;
	}
}

void DragEngine::MouseEvent(MouseEventType, float value)
{
}

void DragEngine::MouseMove(float x, float y)
{
}

void DragEngine::AddTickableObject(TickableObject * object)
{
	allTickables.push_back(object);
}

void DragEngine::AddSceneObject(SceneObject * object)
{
	if (object->GetCollision())object->GetCollision()->SetBoundsBySceneObject();
	// add a should ever tick option in SceneObject for optimization
	//i.e. if(object->shouldEverTick)
	AddTickableObject((TickableObject*)(object));
	mainRenderEngine->AddSceneObject(object);
	physicsEngine->AddPhysicsObject(object->GetPhysics());
	CollisionObjectBase* collision = object->GetCollision();
	if (collision)GetPhysicsEngine()->AddCollisionObject(collision);
}

void DragEngine::DestroySceneObject(SceneObject * object)
{
	mainRenderEngine->RemoveSceneObject(object);
	delete object;
	delete object->GetPhysics();
}

void DragEngine::RemoveTickableObject(TickableObject * object)
{
	remove(allTickables,object);
}

const char * DragEngine::GetVersion()
{
	return Version;
}

void DragEngine::GetVersion(unsigned &Major, unsigned &Minor, unsigned &Revision)
{
	Major = VERSION_MAJOR;
	Minor = VERSION_MINOR;
	Revision = VERSION_REVISION;
}

ThreadBase * DragEngine::CreateThread()
{
	ThreadBase* thread = 0;
#ifdef _WIN32
	thread = new WindowsThread();
#else
	throw std::exception("Threads Not Implented For This Platform !");
#endif
	return thread;
}
