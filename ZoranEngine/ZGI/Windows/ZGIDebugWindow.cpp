#include "stdafx.h"
#include "ZGIDebugWindow.h"

#include <string>

#include <ZGI/Panels/ZGIGridPanel.h>
#include <ZGI/Panels/ZGIListPanel.h>
#include <ZGI/Panels/ZGICollapsibleListPanel.h>
#include <ZGI/Widgets/ZGILabel.h>
#include <ZGI/Widgets/ZGIImage.h>
#include <Core/Resources/ResourceManager.h>

#include <Rendering/Primitives.h>

ZGIDebugWindow::ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : ZGIVirtualWindow(pos,size,OSWindowSize,parent)
{
	ResourceManager *man = RM;
	FontResource* font = man->FontForZFT("arial-msdf.zft");
	ZGICollapsibleListPanel* list = new ZGICollapsibleListPanel(this);

	list->SetListMaxSize(3);
	
	for (unsigned i = 0; i < 2; i++)
	{
		ZGIImage* image = new ZGIImage(this);
		image->SetImage("grid.png");

		list->AddWidget(image);
	}

	SetRootContent(list);
	SetBackgroundColor({ 0.1f,0.1f,0.1f,0.6f });
}


ZGIDebugWindow::~ZGIDebugWindow()
{
}
