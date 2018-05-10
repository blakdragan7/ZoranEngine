#include "stdafx.h"

#include "DragEngine.h"
#include "WindowBase.h"

DragEngine::DragEngine()
{
}

DragEngine::~DragEngine()
{
	if (mainWindow)delete mainWindow;
	mainWindow = 0;
}

int DragEngine::MainLoop()
{
	if (mainWindow)mainWindow->MainDraw();
	return 0;
}
