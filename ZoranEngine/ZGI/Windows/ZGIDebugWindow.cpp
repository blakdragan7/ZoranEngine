#include "stdafx.h"
#include "ZGIDebugWindow.h"

#include <string>
#include <sstream>
#include <iomanip>

#include <ZGI/Panels/ZGITreePanel.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Panels/ZGIGridPanel.h>

#include <Rendering/Primitives.h>

ZGIDebugWindow::ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : ZGIVirtualWindow(pos,size,OSWindowSize,parent)
{
	isResizable = true;

	fpsLabel = new ZGILabel(this);
	fpsLabel->SetText("fps: 0.0");
	fpsLabel->SetFontSize(size.h * 0.1f);

	tree = new ZGITreePanel(30,this);
	
	grid = new ZGIGridPanel(10, 10, this);

	grid->AddWidget(fpsLabel, 0, 9, 10, 1);
	grid->AddWidget(tree,  0,0,9,9);

	SetRootContent(grid);
	SetBackgroundColor({ 0.1f,0.1f,0.1f,0.6f });

	tree->SetSocketSize(30);
}

ZGIDebugWindow::~ZGIDebugWindow()
{
	delete grid;
}

void ZGIDebugWindow::SetFPS(double fps)
{
	// unsafe for threaded application but faster for single threaded
	static std::stringstream ss;
	ss.str("");
	ss.clear();
	ss << "fps: " << std::fixed << std::setprecision(2) << fps;
	std::string text = ss.str();
	fpsLabel->SetText(text);
}
