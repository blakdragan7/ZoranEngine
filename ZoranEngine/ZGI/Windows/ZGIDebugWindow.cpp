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
	fpsLabel = new ZGILabel(this);
	fpsLabel->SetText("fps: 0.0");
	fpsLabel->SetFontSize(size.h * 0.1f);

	tree = new ZGITreePanel(this);
	
	grid = new ZGIGridPanel(10, 10, this);
	grid->AddWidget(fpsLabel, 0, 9,10);
	grid->AddWidget(tree,  1,0,9,9);

	SetRootContent(grid);
	SetBackgroundColor({ 0.1f,0.1f,0.1f,0.6f });
}

ZGIDebugWindow::~ZGIDebugWindow()
{
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
