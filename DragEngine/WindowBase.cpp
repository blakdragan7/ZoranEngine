#include "stdafx.h"

#include "WindowBase.h"
#include "RenderEngine.h"

WindowBase::WindowBase(RenderEngine* engine)
{
	this->renderEngine = engine;
	isFullScreen = false;
	isMaximized = false;
}

WindowBase::~WindowBase()
{
	if (renderEngine)delete renderEngine;
}

void WindowBase::MakeWindow(const char * title, Vec2I position, Vec2I size)
{
	this->MakeWindow(title,position.x, position.y,size.x,size.y);
}

void WindowBase::MainDraw()
{
	renderEngine->ClearBuffers();
	renderEngine->DrawAll();

	SwapBuffers();
}
