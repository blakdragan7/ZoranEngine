#include "pch.h"
#include "ZoranEditorMainWindow.h"
#include <ZGI/Panels/ZGITabPanel.h>
#include <ZGI/Widgets/ZGIGameView.h>

ZoranEditorMainWindow::ZoranEditorMainWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : ZGIVirtualWindow(pos, size, OSWindowSize, parent)
{
	mainPanel = SpawnWidget<ZGITabPanel>();
	GameView = SpawnWidget<ZGIGameView>();

	mainPanel->AddTab("GameView", GameView);
	mainPanel->SetIndexActive(0);

	SetRootContent(mainPanel);
}

ZoranEditorMainWindow::~ZoranEditorMainWindow()
{
}
