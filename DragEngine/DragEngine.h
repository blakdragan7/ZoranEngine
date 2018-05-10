#pragma once
#include "PlatformTypes.h"

class WindowBase;
class DRAGENGINE_EXPORT DragEngine
{
private:
	WindowBase* mainWindow;

public:
	DragEngine();
	~DragEngine();

	int MainLoop();
};

