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

#include <Utils/Statistics.h>
#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include <Core/Audio/AL/OALAudioEngine.h>

ZoranEngine* ZoranEngine::instance = 0;

bool ZoranEngine::canRenderDebug = false;

ZoranEngine::ZoranEngine()
{
	audioEngine = 0;
	mainWindow = 0;
	if (instance)throw std::exception("There can only be one ZoranEngine instance !");
	instance = this;
	shouldRun = true;
	physicsEngine = new PhysicsEngine();
	mainRenderEngine = 0;
	isPaused = false;
	logger = new ConsoleLogger();
	logger->SetLogLevel(LogLevel_Error);
	step = false;
	camera = 0;

	defaultAllocator = new CAllocator();

	allSceneObjects = new std::vector<SceneObject*>();
	allTickables = new std::vector<TickableObject*>();
}

ZoranEngine::~ZoranEngine()
{
	if (audioEngine)delete audioEngine;

	for (auto object : *allSceneObjects)
	{
		object->Destroy();
	}

	delete allSceneObjects;
	delete allTickables;

	mainWindow->renderEngine = 0;

	if (physicsEngine)delete physicsEngine;
	if (mainRenderEngine)delete mainRenderEngine;
	if (mainWindow)delete mainWindow;

	delete defaultAllocator;

	delete logger;
}

int ZoranEngine::MainLoop()
{
	// Each Os will have it's own main loop
	// should probably move these to there own functions
#ifdef _WIN32
	MSG       msg = { 0 };

	double statistics = 0;
	static HighPrecisionClock cl;
	cl.TakeClock();

	while (WM_QUIT != msg.message && shouldRun)
	{
		// Main body of the Demo window starts here.

		DEBUG_BENCH_START;

		double deltaTime = (cl.GetDiffSeconds());
		deltaTime = min(1.0f / 60.0f , deltaTime);
		
		cl.TakeClock();

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
			if (physicsEngine)physicsEngine->UpdateAll(static_cast<float>(deltaTime));

			for (auto object : *allTickables)
			{
				object->Tick(static_cast<float>(deltaTime));
			}
		}
		else
		{

		}

		if (mainWindow)mainWindow->MainDraw();
	}
#endif

	return 0;
}

bool ZoranEngine::Init()
{
	Random::Init();
	
	mainRenderEngine = new OpenGLRenderEngine();
	WindowBase* window;
#ifdef _WIN32
	window = new WindowsWindow(this);
	window->MakeWindow("test", 0, 0, 1920, 1080);
#endif
	mainRenderEngine->InitEngine(window->GetHandle());

	mainWindow = window;

	audioEngine = new OALAudioEngine();
	audioEngine->Init(NULL);

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
			case 'B':
				std::cout << *BenchMarker::Singleton() << std::endl;
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
	AddTickableObject(object);
}

void ZoranEngine::DestroySceneObject(SceneObject * object)
{
	remove(*allSceneObjects, object);
	RemoveTickableObject(object);
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
