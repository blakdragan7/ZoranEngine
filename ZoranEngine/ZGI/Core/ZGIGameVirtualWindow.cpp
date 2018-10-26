#include "stdafx.h"
#include "ZGIGameVirtualWindow.h"

#include <Core/CameraBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Core/PlayerInstanceBase.h>
#include <Rendering/TextureBase.h>
#include <Rendering/Renderers/TriangleStripRenderer.h>

#include <Rendering/OpenGL/2D/StandardShader2D.h>

ZGIGameVirtualWindow::ZGIGameVirtualWindow(Vec2D pos, Vec2D size, Vec2I OSWindowSize, bool is3D, ZGIVirtualWindow* parent) : player(0), is3DCache(is3D), ZGIVirtualWindow(pos,size,OSWindowSize,parent)
{
	fullScreenProgram = rEngine->CreateShaderProgram<StandardShader2D>();
	fullScreenRenderer = rEngine->CreateTriangleStripRenderer();
	fullScreenRenderer->MakeFullScreenQuad();
}

ZGIGameVirtualWindow::~ZGIGameVirtualWindow()
{
	if (player)delete player;
	delete fullScreenRenderer;
}

void ZGIGameVirtualWindow::RenderWindow(Vec2D globalOffset)
{
	if (player)
	{
		player->RenderPlayer();
	
		if (is3DCache)rEngine->EnableDepthTesting();

		player->RenderPlayer();

		if (is3DCache)rEngine->DisableDepthTesting();

		if (const TextureBase* cameraTerxture = player->GetPlayerCamera()->GetCameraTexture())
		{
			rEngine->ClearBuffers();
			fullScreenProgram->BindProgram();
			fullScreenProgram->SetMatricies(Matrix44::IdentityMatrix, Matrix44::IdentityMatrix);
			cameraTerxture->UseTexture(0);
			fullScreenRenderer->RenderObject(Matrix44::IdentityMatrix);
		}
	}

	/* any post processing */

	/* render the UI */

	ZGIVirtualWindow::RenderWindow(globalOffset);
}

void ZGIGameVirtualWindow::OSWindowWasResized(Vec2I newSize)
{
	if (player)player->WindowResizedView((float)newSize.w, (float)newSize.h);
}
