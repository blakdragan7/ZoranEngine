#include "stdafx.h"
#include "ZGIImage.h"

#include <Rendering/RenderEngineBase.h>
#include <Resources/ImageResource.h>
#include <Resources/ResourceManager.h>

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

	foregroundBrush->SetBackgroudImage(RM->ImageForPath(imagePath));
}

void ZGIImage::SetForegroundImage(ImageResource image)
{
	foregroundBrush->SetBackgroudImage(image);
	hasForgroundImage = image.IsValid();
}

void ZGIImage::SetImage(const char * imagePath)
{
	auto image = RM->ImageForPath(imagePath);
	widgetBrush->SetBackgroudImage(image);
	bounds = image->GetSize();
}

void ZGIImage::SetImage(ImageResource image)
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
