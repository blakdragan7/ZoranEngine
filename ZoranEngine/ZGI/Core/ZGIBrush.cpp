#include "stdafx.h"
#include "ZGIBrush.h"

#include <Rendering/Renderers/GUIRenderer.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/TextureBase.h>

ZGIBrush::ZGIBrush() : backgroundImage(0)
{
	renderer = rEngine->CreateGUIRenderer();
	renderer->SetHasTexture(false);
	renderer->SetTint({ 0,0,0,0 });
}

ZGIBrush::~ZGIBrush()
{
	delete renderer;
	if (backgroundImage)TextureManager::GetInstance()->DestroyTexture(backgroundImage);
}

void ZGIBrush::SetBackgroudImage(std::string texture)
{
	backgroundImage = TextureManager::GetInstance()->TextureForFilePath(texture.c_str());
	renderer->SetHasTexture(true);
}

void ZGIBrush::SetBackgroudImage(TextureBase * texture)
{
	if (backgroundImage == texture || texture == 0)return;

	backgroundImage = texture;
	renderer->SetHasTexture(true);
}

void ZGIBrush::SetBackgroudHue(const Color & hue)
{
	renderer->SetTint(hue);
}

void ZGIBrush::RenderBrush(const Matrix44& matrix)
{
	if(backgroundImage)backgroundImage->UseTexture(0);

	renderer->RenderObject(matrix);
}
