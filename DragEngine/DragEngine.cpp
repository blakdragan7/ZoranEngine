#include "stdafx.h"

#include "DragEngine.h"
#include "WindowBase.h"
#include "WindowsWindow.h"
#include "OpenGLRenderEngine.h"

DragEngine::DragEngine()
{
	mainWindow = 0;
}

DragEngine::~DragEngine()
{
	if (mainWindow)delete mainWindow;
	mainWindow = 0;
}

int DragEngine::MainLoop()
{
#ifdef _WIN32
	MSG       msg = { 0 };

	while (WM_QUIT != msg.message)
	{

		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		if (mainWindow)mainWindow->MainDraw();
	}
#endif

	return 0;
}

bool DragEngine::Init()
{
	OpenGLRenderEngine* renderEngine = new OpenGLRenderEngine();
	WindowsWindow* window = new WindowsWindow(renderEngine);
	window->MakeWindow("test", 0, 0, 1920, 1080);
	window->SetWindowFullScreen(true);
	renderEngine->InitEngine(window->GetHandle());

	mainWindow = window;
	return true;
}
