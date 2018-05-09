#include "stdafx.h"

#include "WindowBase.h"
#include "RenderEngine.h"

WindowBase::WindowBase(std::shared_ptr<RenderEngine> const&engine)
{
	this->engine = engine;
}

WindowBase::~WindowBase()
{
}

void WindowBase::MainDraw()
{
	engine->ClearBuffers();
	engine->DrawAll();

	SwapBuffers();
}
