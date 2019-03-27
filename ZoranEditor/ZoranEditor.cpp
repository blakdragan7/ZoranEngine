// ZoranEditor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <Core/ZoranEngine.h>

#include "ZoranEditorMainWindow.h"

int main()
{
	ZoranEngine engine;

	ZoranEditorMainWindow* rootWindow = engine.SetRootWindow<ZoranEditorMainWindow>();

	return engine.MainLoop();
}
