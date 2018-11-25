#include "stdafx.h"
#include "ZGIImageWidget.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/TextureBase.h>

#include <ZGI/Core/ZGIBrush.h>
#include <Rendering/Primitives.h>

ZGIImageWidget::ZGIImageWidget(ZGIVirtualWindow* owningWindow) : ZGIWidget(owningWindow)
{
	shouldDrawBrush = true;
}

ZGIImageWidget::~ZGIImageWidget()
{
}

void ZGIImageWidget::SetImage(const char * imagePath)
{
	TextureBase* image = TextureManager::GetInstance()->TextureForFilePath(imagePath, Render_Data_Type_RGBA_32);

	widgetBrush->SetBackgroudImage(image);
	SetBounds(image->GetSize());
}

void ZGIImageWidget::SetImage(TextureBase * image)
{
	widgetBrush->SetBackgroudImage(image);

	SetBounds(image->GetSize());
}

void ZGIImageWidget::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Some Ancher stuff
}
