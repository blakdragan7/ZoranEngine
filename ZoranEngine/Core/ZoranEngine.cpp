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
		
		gameWindow->SetFPS(1.0 / deltaTime);
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

void ZoranEngine::CreateGameModeWindows(bool is3D)
{
	gameWindow = mainWindow->SetRootVirtualWindow<ZGIGameVirtualWindow>(Vector2D(0,0), mainWindow->GetSize(), mainWindow->GetSize(),is3D);
	gameWindow->SetPlayerInstance(mainPlayer);

	//debugWindow = vWindow->AddSubWindow<ZGIDebugWindow>(Vector2D(600,0), Vector2D(600,900), mainWindow->GetSize());
}

void ZoranEngine::Setup2DScene(float centerx, float centery, float width, float height)
{
	main2DRenderEngine = new OpenGL2DRenderEngine();
	main2DRenderEngine->InitEngine(mainWindow->GetHandle());

	physicsEngine->SetupFor2D({ centerx, centery }, { width, height });

	mainPlayer = new DebugPlayerInstance(new OrthoCamera("camera", width, height, 0));
	mainPlayer->TranslateView(Vector2D(centerx, centery));
	mainPlayer->WindowResizedView(mainWindow->GetSize());

	FrameBufferBase* frameBuffer;
	TextureBase* texture;
	main2DRenderEngine->CreateFrameBuffer(&frameBuffer,&texture,mainWindow->GetSize());

	frameBuffer->SetRenderFunction([](const Matrix44& cameraMatrix){
		rEngine->DrawScene(cameraMatrix);
	});

	mainPlayer->SetCameraSceneBuffer(frameBuffer);
}

void ZoranEngine::Setup2DScene(Vector2D center, Vector2D size)
{
	main2DRenderEngine = new OpenGL2DRenderEngine();
	main2DRenderEngine->InitEngine(mainWindow->GetHandle());

	physicsEngine->SetupFor2D(center, size);

	mainPlayer = new DebugPlayerInstance(new OrthoCamera("camera", size.w, size.h, 0));
	mainPlayer->TranslateView(center);
	mainPlayer->WindowResizedView(mainWindow->GetSize());

	FrameBufferBase* frameBuffer;
	TextureBase* texture;
	main2DRenderEngine->CreateFrameBuffer(&frameBuffer, &texture, mainWindow->GetSize());

	frameBuffer->SetRenderFunction([](const Matrix44& cameraMatrix) {
		rEngine->DrawScene(cameraMatrix);
	});

	mainPlayer->SetCameraSceneBuffer(frameBuffer);
}

void ZoranEngine::Setup3DScene(Vector3D center, Vector3D size, float fov, float nearp, float farp)
{
	main3DRenderEngine = new OpenGL3DRenderEngine();
	main3DRenderEngine->InitEngine(mainWindow->GetHandle());
	is3D = true;

	physicsEngine->SetupFor3D(center, size);
	
	mainPlayer = new DebugPlayerInstance(new PerspectiveCamera("camera", fov, size.x / size.y, nearp, farp));
	mainPlayer->TranslateView(center);
	mainPlayer->WindowResizedView(mainWindow->GetSize());

	FrameBufferBase* frameBuffer;
	TextureBase* texture;
	main3DRenderEngine->CreateFrameBuffer(&frameBuffer, &texture, { 1920,1080 });

	frameBuffer->SetRenderFunction([](const Matrix44& cameraMatrix) {
		rEngine->DrawScene(cameraMatrix);
	});

	mainPlayer->SetCameraSceneBuffer(frameBuffer);
}

void ZoranEngine::DrawStep()
{
	DEBUG_TAKE_BENCH;

	GetRenderer()->DrawDebugGUI();
}

void ZoranEngine::KeyEvent(KeyEventType type, unsigned key)
{
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
			case Key_Esc:
				shouldRun = false;
				break;
			case 'P':
				debugWindow->GetTree()->Print(0);
				//if (physicsEngine)physicsEngine->GetCollisionBucketRoot()->PrintAllContents();
				//if (physicsEngine)physicsEngine->GetCollisionBucketRoot()->PrintAllCollisions();
				break;
			case 'S':
				if (isPaused)step = true;
				break;
			case 'B':
				std::cout << *BenchMarker::Singleton() << std::endl;
				break;
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
