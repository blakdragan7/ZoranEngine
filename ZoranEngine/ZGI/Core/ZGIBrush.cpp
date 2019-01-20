#include "stdafx.h"
#include "ZGIBrush.h"

#include <Rendering/Renderers/GUIRenderer.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureBase.h>

#include <Resources/ResourceManager.h>

ZGIBrush::ZGIBrush()
{
	renderer = rEngine->CreateGUIRenderer();
	renderer->SetHasTexture(false);
	renderer->SetTint({ 0,0,0,0 });
}

ZGIBrush::~ZGIBrush()
{
	delete renderer;
}

void ZGIBrush::SetBackgroudImage(const char * texture)
{
	backgroundImage = RM->ImageForPath(texture);
	renderer->SetHasTexture(true);
}

void ZGIBrush::SetBackgroudImage(std::string texture)
{
	backgroundImage = RM->ImageForPath(texture);
	renderer->SetHasTexture(true);
}

void ZGIBrush::SetBackgroudImage(ImageResource texture)
{
	if (backgroundImage == texture)return;

	backgroundImage = texture;
	renderer->SetHasTexture(texture.IsValid());
}

void ZGIBrush::SetBackgroudHue(const Color & hue)
{
	renderer->SetTint(hue);
}

void ZGIBrush::RenderBrush(const Matrix44& matrix)
{
	if(backgroundImage.IsValid())backgroundImage->UseTexture(0);

	renderer->RenderObject(matrix);
}
