#include "stdafx.h"
#include "ZGIGameView.h"

#include <ZGI/Core/ZGIVirtualViewport.h>
#include <ZGI/Core/ZGIBrush.h>
#include <ZGI/Windows/ZGIVirtualWindow.h>
#include <Core/CameraBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Core/PlayerInstanceBase.h>
#include <Rendering/TextureBase.h>

#include <Rendering/OpenGL/2D/StandardShader2D.h>

ZGIGameView::ZGIGameView(ZGIVirtualWindow* owningWindow) : ZGIWidget(owningWindow)
{
	player = 0;
	viewport = new ZGIVirtualViewport(Vector2D::Zero, GetSize(), owningWindow->GetWindowSize());
}

ZGIGameView::~ZGIGameView()
{
	delete viewport;
}

void ZGIGameView::SetPlayer(PlayerInstanceBase * instance)
{
	player = instance;
}

void ZGIGameView::Render(const Matrix44 & projection)
{
	if (player)
	{
		viewport->SetViewportActive(Vector2D::Zero);

		player->RenderPlayer();

		const ImageResource cameraTerxture = player->GetPlayerCamera()->GetCameraTexture();
		if (cameraTerxture.IsValid())
		{
			rEngine->ClearBuffers();
			GetBrush()->SetBackgroudImage(cameraTerxture);
		}
	}

	ZGIWidget::Render(projection);
}

ZGIWidget * ZGIGameView::HitTest(Vec2D pos)
{
	return ZGIWidget::HitTest(pos);
}

void ZGIGameView::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher stuff
}
