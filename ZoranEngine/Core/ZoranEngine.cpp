#include "stdafx.h"

#include <Utils/Random.h>
#include <Core/ZoranEngine.h>
#include <Core/WindowBase.h>
#include <Core/TickableObject.h>
#include <Core/2D/OrthoCamera.h>
#include <Core/3D/PerspectiveCamera.h>
#include <Physics/PhysicsEngine.h>
#include <Windows/WindowsWindow.h>
#include <Utils/HighPrecisionClock.h>

#include <Rendering/OpenGL/2D/OpenGL2DRenderEngine.h>
#include <Rendering/OpenGL/3D/OpenGL3DRenderEngine.h>

#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/Collision/CollisionBucketBase.h>

#include <Core/Allocators/CAllocator.h>

#include <iostream>

#include "SceneObject.h"

#include "Utils/VectorAddons.hpp"

#include "Version.h"
#include "Utils/ConsoleLogger.h"

#include <Utils/Statistics.h>
#include <ThirdParty/imgui/imgui.h>
#include <ThirdParty/imgui/imgui_impl_opengl3.h>

#include <Core/Audio/AL/OALAudioEngine.h>

ZoranEngine* ZoranEngine::instance = 0;

bool ZoranEngine::canRenderDebug = false;

ZoranEngine::ZoranEngine()
{
	main2DRenderEngine = 0;
	main3DRenderEngine = 0;

	is3D = false;
	audioEngine = 0;
	mainWindow = 0;
	if (instance)throw std::exception("There can only be one ZoranEngine instance !");
	instance = this;
	shouldRun = true;
	physicsEngine = new PhysicsEngine();
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

	if (physicsEngine)delete physicsEngine;
	if (main2DRenderEngine)delete main2DRenderEngine;
	if (main3DRenderEngine)delete main3DRenderEngine;
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
	
	WindowBase* window;
#ifdef _WIN32
	window = new WindowsWindow(this);
	window->MakeWindow("test", 0, 0, 1920, 1080);
#endif
	mainWindow = window;

	audioEngine = new OALAudioEngine();
	audioEngine->Init(NULL);

	return true;
}

void ZoranEngine::Setup2DScene(float centerx, float centery, float width, float height)
{
	main2DRenderEngine = new OpenGL2DRenderEngine();
	main2DRenderEngine->InitEngine(mainWindow->GetHandle());

	physicsEngine->SetupFor2D(Vec2D(centerx, centery), Vec2D(width, height));

	camera = new OrthoCamera("camera", width, height, 0);
	camera->Translate(centerx, centery, 0);
	camera->ScreenResized(mainWindow->GetSize());
}

void ZoranEngine::Setup2DScene(Vector2D center, Vector2D size)
{
	main2DRenderEngine = new OpenGL2DRenderEngine();
	main2DRenderEngine->InitEngine(mainWindow->GetHandle());

	physicsEngine->SetupFor2D(center, size);

	camera = new OrthoCamera("camera",size.x,size.y, 0);
	camera->Translate(center.x, center.y, 0);
	camera->ScreenResized(mainWindow->GetSize());
}

void ZoranEngine::Setup3DScene(Vector3D center, Vector3D size, float fov, float nearp, float farp)
{
	is3D = true;
	main3DRenderEngine = new OpenGL3DRenderEngine();
	main3DRenderEngine->InitEngine(mainWindow->GetHandle());

	physicsEngine->SetupFor3D(center, size);
	
	camera = new PerspectiveCamera("camera", fov, size.x / size.y, nearp, farp);
	camera->Translate(center);
	camera->ScreenResized(mainWindow->GetSize());
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

inline RenderEngineBase * ZoranEngine::GetRenderer() const
{
	if (is3D)
		return main3DRenderEngine;
	else
		return main2DRenderEngine;
}

const char * ZoranEngine::GetVersion()const
{
	return Version;
}

void ZoranEngine::GetVersion(unsigned &Major, unsigned &Minor, unsigned &Revision)const
{
	Major = VERSION_MAJOR;
	Minor = VERSION_MINOR;
	Revision = VERSION_REVISION;
}
