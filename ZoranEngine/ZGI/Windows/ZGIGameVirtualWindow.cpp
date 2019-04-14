#include "stdafx.h"
#include "ZGIGameVirtualWindow.h"

#include <ZGI/Widgets/ZGIGameView.h>

#include <ZGI/Widgets/ZGILabel.h>
#include <Core/CameraBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Core/PlayerInstanceBase.h>
#include <Rendering/TextureBase.h>
#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/OpenGL/2D/StandardShader2D.h>

#include <Resources/ImageResource.h>

ZGIGameVirtualWindow::ZGIGameVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, bool is3D, ZGIVirtualWindow* parent) : ZGIVirtualWindow(pos,size,OSWindowSize,parent)
{
	gameView = SpawnWidget<ZGIGameView>();

	SetRootContent(gameView);
}

ZGIGameVirtualWindow::~ZGIGameVirtualWindow()
{
}

void ZGIGameVirtualWindow::RenderWindow(Vec2D globalOffset)
{
	gameView->Render(Matrix44::IdentityMatrix);

	ZGIVirtualWindow::RenderWindow(globalOffset);
}

void ZGIGameVirtualWindow::OSWindowWasResized(Vec2I newSize)
{
	ZGIVirtualWindow::OSWindowWasResized(newSize);
}
