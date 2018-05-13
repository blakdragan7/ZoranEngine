#include "stdafx.h"

#include "DragEngine.h"
#include "WindowBase.h"
#include "WindowsWindow.h"
#include "OpenGLRenderEngine.h"

#include <iostream>

DragEngine* DragEngine::instance = 0;

DragEngine::DragEngine()
{
	mainWindow = 0;
	if (instance)throw std::exception("There can only be one DragEngine instance !");
	instance = this;
	shouldRun = true;
}

DragEngine::~DragEngine()
{
	if (mainWindow)delete mainWindow;
	mainWindow = 0;
}

int DragEngine::MainLoop()
{
	// Each Os will have it's own main loop
	// should probably move these to there own functions
#ifdef _WIN32
	MSG       msg = { 0 };

	while (WM_QUIT != msg.message && shouldRun)
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
	renderEngine->InitEngine(window->GetHandle());

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
		if (key == F11_Key)mainWindow->SetWindowFullScreen(!mainWindow->IsFullScreen());
		break;
	}
}

void DragEngine::MouseEvent(MouseEventType, float value)
{
}

void DragEngine::MouseMove(float x, float y)
{
}
