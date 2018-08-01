#include "stdafx.h"

#include "WindowBase.h"
#include "Rendering/RenderEngineBase.h"

WindowBase::WindowBase(ZoranEngine* zoranEngine)
{
	this->engine = zoranEngine;
	this->renderEngine = zoranEngine->GetRenderer();

	isFullScreen = false;
	isMaximized = false;
	windowHandle = 0;
}

WindowBase::~WindowBase()
{
}

void WindowBase::Resize(Vec2L size)
{
	SetWindowSizeNoExecute(size.w,size.h);
	renderEngine->Resize(size.w, size.h);
	engine->ScreenResized(size.w, size.h);
}

void WindowBase::SetPosition(Vec2L position)
{
	this->SetPosition(position.x,position.y);
}

void WindowBase::SetSize(Vec2L size)
{
	this->SetSize(size.w, size.h);
}

void WindowBase::MakeWindow(const char * title, Vec2L position, Vec2L size)
{
	this->MakeWindow(title,position.x, position.y,size.w,size.h);
}

void WindowBase::MainDraw()
{
	renderEngine->ClearBuffers();
	renderEngine->DrawAll();
	SwapBuffers();
}
