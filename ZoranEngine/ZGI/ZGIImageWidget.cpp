#include "stdafx.h"
#include "ZGIImageWidget.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureBase.h>
#include <Rendering/Renderers/GUIRenderer.h>
#include <Rendering/TextureManager.h>


ZGIImageWidget::ZGIImageWidget()
{
	image = 0;
	renderer = rEngine->CreateGUIRenderer();
}

ZGIImageWidget::~ZGIImageWidget()
{
	if (image)TextureManager::GetInstance()->DestroyTexture(image);
	delete renderer;
}

void ZGIImageWidget::SetImage(const char * imagePath)
{
	if (image)TextureManager::GetInstance()->DestroyTexture(image);

	image = TextureManager::GetInstance()->TextureForFilePath(imagePath);
}

void ZGIImageWidget::SetImage(TextureBase * image)
{
	if (this->image)TextureManager::GetInstance()->DestroyTexture(this->image);

	this->image = image;
}

void ZGIImageWidget::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Some Ancher stuff
}

void ZGIImageWidget::Render(const Matrix44 & projection)
{
	if (image)image->UseTexture(0);
	renderer->RenderObject(projection * modelCache);
}
