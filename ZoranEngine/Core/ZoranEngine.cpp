#include "stdafx.h"

#include <Utils/Random.h>
#include <Core/ZoranEngine.h>
#include <Core/WindowBase.h>
#include <Interfaces/ITickableObject.h>
#include <Core/2D/OrthoCamera.h>
#include <Core/3D/PerspectiveCamera.h>
#include <Physics/PhysicsEngine.h>
#include <Windows/WindowsWindow.h>
#include <Utils/HighPrecisionClock.h>

#include <ZGI/Panels/ZGITreePanel.h>

#include <ZGI/Windows/ZGIGameVirtualWindow.h>
#include <ZGI/Windows/ZGIDebugWindow.h>

#include <Math/Matrix44.h>

#include <Rendering/TextureBase.h>

#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/ShaderProgramBase.h>

#include <Rendering/FrameBufferBase.h>
// TODO remove these and make a way for the user to decide which engine to use
#include <Rendering/OpenGL/2D/OpenGL2DRenderEngine.h>
#include <Rendering/OpenGL/3D/OpenGL3DRenderEngine.h>

#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/Collision/CollisionBucketBase.h>

#include <iostream>

#include <Core/SceneObject.h>

#include <Utils/VectorAddons.hpp>

#include <Version.h>
#include <Utils/ConsoleLogger.h>
#include <Utils/FileLogger.h>

#include <Utils/Statistics.h>

#include <Audio/AL/OALAudioEngine.h>

#include <Core/PlayerInstanceBase.h>
#include <Core/DebugPlayerInstance.h>

#include <Resources/ResourceManager.h>
#include <Resources/FontResource.h>

#include <Rendering/Renderers/FontRenderer.h>

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
	//logger = new FileLogger("output.log");
#ifdef _DEBUG
	logger->SetLogLevel(LogLevel_Debug);
#else
	logger->SetLogLevel(LogLevel_Error);
#endif
	step = false;

	allSceneObjects = new std::vector<SceneObject*>;
	allTickables = new std::vector<ITickableObject*>;
}

ZoranEngine::~ZoranEngine()
{
	mainWindow->StopEvents();

	if (audioEngine)delete audioEngine;
	audioEngine = 0;

	for (auto object : *allSceneObjects)
	{
		delete object;
	}

	delete allSceneObjects;
	delete allTickables;

	if (physicsEngine)delete physicsEngine;
	if (main2DRenderEngine)delete main2DRenderEngine;
	if (main3DRenderEngine)delete main3DRenderEngine;
	if (mainWindow)delete mainWindow;
	delete logger;

	allSceneObjects = 0;
	allTickables = 0;

	physicsEngine = 0;
	main2DRenderEngine = 0;
	main3DRenderEngine = 0;
	mainWindow = 0;
	logger = 0;
}

int ZoranEngine::MainLoop()
{
	// Each OS will have it's own main loop
	// TODO: move each OS implementation to there own functions
#ifdef _WIN32
	MSG       msg = { 0 };

	double statistics = 0;
	HighPrecisionClock cl;
	cl.TakeClock();

	while (WM_QUIT != msg.message && shouldRun)
	{
		DEBUG_BENCH_START;

		double deltaTime = (cl.GetDiffSeconds());
		float dt = static_cast<float>(deltaTime);
		
		deltaTime = min(1.0f / 60.0f, deltaTime);

		cl.TakeClock();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

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

		if (mainWindow)mainWindow->MainDraw(dt);
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

void ZoranEngine::SetRootWindow(ZGIVirtualWindow * rootWindow)
{
	if (mainWindow)
	{
		mainWindow->SetRootVirtualWindow(rootWindow);
	}
	else
	{
		LOG_ERROR << "Can not set root window because platform window mainWindow NULL\n";
	}
}

void ZoranEngine::DrawStep()
{
	DEBUG_TAKE_BENCH;
}

void ZoranEngine::KeyEvent(KeyEventType type, unsigned key)
{
	return;
	switch (type)
	{
	case KeyEventType_Key_Down:
		switch (key)
		{
			case Key_Space:
			{
				isPaused = !isPaused;
				break;
			}
		}
		break;
	case KeyEventType_Key_Up:
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
}

void ZoranEngine::AddTickableObject(ITickableObject * object)
{
	allTickables->push_back(object);
}

void ZoranEngine::AddSceneObject(SceneObject * object)
{
	allSceneObjects->push_back(object);
	if(object->GetWillEverTick())
		AddTickableObject(object);
}

void ZoranEngine::CleanObjects()
{
	for (size_t i = 0; i < allSceneObjects->size();)
	{
		auto object = (*allSceneObjects)[i];
		if (object->GetIsValid() == false)
		{
			delete object;
			allSceneObjects->erase(allSceneObjects->begin() + i);
		}
		else
		{
			++i;
		}
	}
}

void ZoranEngine::DestroySceneObject(SceneObject * object)
{
	RemoveTickableObject(object);
	object->MarkInvalid();
}

void ZoranEngine::RemoveTickableObject(ITickableObject * object)
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
