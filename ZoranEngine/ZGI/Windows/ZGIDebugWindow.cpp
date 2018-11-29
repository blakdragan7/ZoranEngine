#include "stdafx.h"
#include "ZGIDebugWindow.h"

#include <ZGI/Widgets/ZGILabel.h>
#include <Core/Resources/ResourceManager.h>

#include <Rendering/Primitives.h>

ZGIDebugWindow::ZGIDebugWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, ZGIVirtualWindow* parent) : ZGIVirtualWindow(pos,size,OSWindowSize,parent)
{
	ResourceManager *man = RM;

	isResizable = true;
	label = new ZGILabel(man->FontForZFT("arial-msdf.zft"),this);
	label->SetFontSize(40);
	label->SetText("This is a test !");
	SetRootContent(label);

	SetBackgroundColor({ 0.1f,0.1f,0.1f,0.6f });
}


ZGIDebugWindow::~ZGIDebugWindow()
{
}
