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

void WindowBase::Resize(Vec2I size)
{
	SetWindowSizeNoExecute(size.w,size.h);
	if(renderEngine)renderEngine->Resize(size.w, size.h);
	engine->ScreenResized(static_cast<float>(size.w), static_cast<float>(size.h));
}

void WindowBase::SetPosition(Vec2I position)
{
	this->SetPosition(position.x,position.y);
}

void WindowBase::SetSize(Vec2I size)
{
	this->SetSize(size.w, size.h);
}

void WindowBase::MakeWindow(const char * title, Vec2I position, Vec2I size)
{
	this->MakeWindow(title,position.x, position.y,size.w,size.h);
}

void WindowBase::MainDraw()
{
	renderEngine->ClearBuffers();
	renderEngine->DrawAll();
	SwapBuffers();
}
