#include "stdafx.h"

#include "WindowBase.h"
#include "RenderEngine.h"

WindowBase::WindowBase(RenderEngine* engine)
{
	this->renderEngine = engine;
}

WindowBase::~WindowBase()
{
	if (renderEngine)delete renderEngine;
}

void WindowBase::MainDraw()
{
	renderEngine->ClearBuffers();
	renderEngine->DrawAll();

	SwapBuffers();
}
