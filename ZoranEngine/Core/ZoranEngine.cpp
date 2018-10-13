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

#include <Math/Matrix44.hpp>

#include <Rendering/TextureBase.h>

#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/ShaderProgramBase.h>

#include <Rendering/FrameBufferBase.h>

#include <Rendering/OpenGL/2D/StandardShader2D.h>
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

#include <Core/PlayerInstanceBase.h>
#include <Core/DebugPlayerInstance.h>

#include <Core/Resources/ResourceManager.h>
#include <Core/Resources/FontResource.h>

#include <Rendering/Renderers/FontRenderer.h>

ZoranEngine* ZoranEngine::instance = 0;

bool ZoranEngine::canRenderDebug = false;

static FontRenderer* fr;

ZoranEngine::ZoranEngine()
{
	main2DRenderEngine = 0;
	main3DRenderEngine = 0;
	fullScreenRenderer = 0;

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
	mainPlayer = 0;

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

	fullScreenProgram = main2DRenderEngine->CreateShaderProgram<StandardShader2D>(StandardShader2D::initMap);

	physicsEngine->SetupFor2D({ centerx, centery }, { width, height });

	mainPlayer = new DebugPlayerInstance(new OrthoCamera("camera", width, height, 0));
	mainPlayer->TranslateView({ centerx, centery });
	mainPlayer->WindowResizedView(mainWindow->GetSize());

	FrameBufferBase* frameBuffer;
	TextureBase* texture;
	main2DRenderEngine->CreateFrameBuffer(&frameBuffer,&texture,mainWindow->GetSize());

	frameBuffer->SetRenderFunction([](const Matrix44& cameraMatrix){
		rEngine->DrawScene(cameraMatrix);
	});

	mainPlayer->SetCameraSceneBuffer(frameBuffer);

	ResourceManager man;
	FontResource* font = man.FontForZFT("arial.zft");
	/*FontResource* font = man.FontForTTF("C:\\Windows\\Fonts\\arial.ttf", 72);
	font->CreateBMPForASCII("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_-+=\\\"';:/?.>,<~` ");
	font->SaveToFile("arial");*/

	fr = rEngine->CreateFontRenderer();

	Font* f = new Font();
	f->fontResource = font;
	f->renderStart.x = 800;
	f->renderStart.y = 450;
	fr->AddAsciiToRender("This Is a Test !", f);
}

void ZoranEngine::Setup2DScene(Vector2D center, Vector2D size)
{
	main2DRenderEngine = new OpenGL2DRenderEngine();
	main2DRenderEngine->InitEngine(mainWindow->GetHandle());

	fullScreenProgram = main2DRenderEngine->CreateShaderProgram<StandardShader2D>(StandardShader2D::initMap);

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

	fullScreenProgram = main3DRenderEngine->CreateShaderProgram<StandardShader2D>(StandardShader2D::initMap);

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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui::NewFrame();

	if (mainPlayer)
	{
		if(is3D)GetRenderer()->EnableDepthTesting();
	
		mainPlayer->RenderPlayer();

		if (is3D)GetRenderer()->DisableDepthTesting();

		if (const TextureBase* cameraTerxture = mainPlayer->GetPlayerCamera()->GetCameraTexture())
		{
			if (fullScreenRenderer == 0)
			{
				fullScreenRenderer = GetRenderer()->CreateTriangleStripRenderer();
				fullScreenRenderer->MakeFullScreenQuad();
			}

			GetRenderer()->ClearBuffers();
			fullScreenProgram->BindProgram();
			fullScreenProgram->SetMatricies(Matrix44::IdentityMatrix, Matrix44::IdentityMatrix);
			cameraTerxture->UseTexture(0);
			fullScreenRenderer->RenderObject(Matrix44::IdentityMatrix);
		}
	}

	static Matrix44 screenMatrix = Matrix44::OrthoMatrix(0, mainWindow->GetSize().x, 0 , mainWindow->GetSize().y,-100,100);

	fr->RenderObject(screenMatrix);

	DEBUG_TAKE_BENCH

	GetRenderer()->DrawDebugGUI();
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
	if(mainPlayer)mainPlayer->WindowResizedView(width, height);
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
