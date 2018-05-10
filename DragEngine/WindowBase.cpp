#include "stdafx.h"

#include "WindowBase.h"
#include "RenderEngine.h"

WindowBase::WindowBase(RenderEngine* engine)
{
	this->engine = engine;
}

WindowBase::~WindowBase()
{
	if (engine)delete engine;
}

void WindowBase::MainDraw()
{
	engine->ClearBuffers();
	engine->DrawAll();

	SwapBuffers();
}
