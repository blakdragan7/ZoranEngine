#include "stdafx.h"

#include "DragEngine.h"
#include "WindowBase.h"
#include "TickableObject.h"
#include "PhysicsEngine.h"
#include "WindowsWindow.h"
#include "OpenGLRenderEngine.h"
#include "HighPrecisionClock.h"
#include <iostream>

#include "Version.h"

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
}

DragEngine::~DragEngine()
{
	if (mainWindow)delete mainWindow;
	if (physicsEngine)delete physicsEngine;
	mainWindow = 0;
}

int DragEngine::MainLoop()
{
	// Each Os will have it's own main loop
	// should probably move these to there own functions
#ifdef _WIN32
	MSG       msg = { 0 };

	HighPrecisionClock clock;

	while (WM_QUIT != msg.message && shouldRun)
	{
		double deltaTime = clock.GetDiffSeconds();
		clock.TakeClock();

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (mainWindow)mainWindow->MainDraw();
		if (physicsEngine)physicsEngine->UpdateAll(deltaTime);
		for (auto object : allSceneObjects)
		{
			object->Tick(deltaTime);
		}

		//std::cout << "Fps: " << 1.0 / deltaTime << std::endl;
	}
#endif

	return 0;
}

bool DragEngine::Init()
{
	mainRenderEngine = new OpenGLRenderEngine();
	WindowsWindow* window = new WindowsWindow(mainRenderEngine);
	window->MakeWindow("test", 0, 0, 1920, 1080);
	mainRenderEngine->InitEngine(window->GetHandle());

	mainWindow = window;

	return true;
}

void DragEngine::KeyEvent(KeyEventType type, unsigned key)
{
	switch (type)
	{
	case KEY_DOWN:
		std::cout << key << " ";
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
	allSceneObjects.push_back(object);
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
