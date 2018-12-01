#include "stdafx.h"
#include "ZGIDebugWindow.h"

#include <string>

#include <ZGI/Panels/ZGIGridPanel.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIImage.h>
#include <Core/Resources/ResourceManager.h>

#include <Rendering/Primitives.h>

ZGIDebugWindow::ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : ZGIVirtualWindow(pos,size,OSWindowSize,parent)
{
	int girdsizex = 9;
	int girdsizey = 9;

	ResourceManager *man = RM;
	FontResource* font = man->FontForZFT("arial-msdf.zft");
	ZGIGridPanel* grid = new ZGIGridPanel(girdsizex, girdsizey,this);

	SetRootContent(grid);

	for (int x = 0; x < girdsizex; x++)
	{
		for (int y = 0; y < girdsizey; y++)
		{
			//ZGIImage * image = new ZGIImage(this);
			//image->SetImage("grid.png");
			//grid->AddWidget(image, x, y);
			ZGILabel* label = new ZGILabel(font, this);
			std::string position = std::to_string(x) + "," + std::to_string(y);
			label->SetText(position);
			label->SetAlignment(Alignment_Center);
			label->SetFontSize(20);
			grid->AddWidget(label, x, y);
		}
	}

	SetBackgroundColor({ 0.1f,0.1f,0.1f,0.6f });
}


ZGIDebugWindow::~ZGIDebugWindow()
{
}
