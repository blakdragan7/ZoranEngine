#include "stdafx.h"
#include "ZGIImage.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/TextureBase.h>

#include <ZGI/Core/ZGIBrush.h>
#include <Rendering/Primitives.h>

ZGIImage::ZGIImage(ZGIVirtualWindow* owningWindow) : hasForgroundImage(false), ZGIWidget(owningWindow)
{
	shouldDrawBrush = true;
	foregroundBrush = new ZGIBrush;
}

ZGIImage::~ZGIImage()
{
	delete foregroundBrush;
}

void ZGIImage::SetHue(const Color & color)
{
	widgetBrush->SetBackgroudHue(color);
}

void ZGIImage::SetForgroundHue(const Color & color)
{
	foregroundBrush->SetBackgroudHue(color);
}

void ZGIImage::SetForegroundImage(const char * imagePath)
{
	if (imagePath == 0)
	{
		hasForgroundImage = false;
		return;
	}

	TextureBase* image = TextureManager::GetInstance()->TextureForFilePath(imagePath, Render_Data_Type_RGBA_32);

	foregroundBrush->SetBackgroudImage(image);
}

void ZGIImage::SetForegroundImage(TextureBase * image)
{
	foregroundBrush->SetBackgroudImage(image);
	hasForgroundImage = image != 0;
}

void ZGIImage::SetImage(const char * imagePath)
{
	TextureBase* image = TextureManager::GetInstance()->TextureForFilePath(imagePath, Render_Data_Type_RGBA_32);

	widgetBrush->SetBackgroudImage(image);
	bounds =image->GetSize();
}

void ZGIImage::SetImage(TextureBase * image)
{
	widgetBrush->SetBackgroudImage(image);

	bounds = image->GetSize();
}

void ZGIImage::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Some Ancher stuff
}

void ZGIImage::Render(const Matrix44 & projection)
{
	ZGIWidget::Render(projection);

	foregroundBrush->RenderBrush(projection * modelCache);
}
