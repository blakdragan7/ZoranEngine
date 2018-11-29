#include "stdafx.h"
#include "ZGIImage.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/TextureManager.h>
#include <Rendering/TextureBase.h>

#include <ZGI/Core/ZGIBrush.h>
#include <Rendering/Primitives.h>

ZGIImage::ZGIImage(ZGIVirtualWindow* owningWindow) : ZGIWidget(owningWindow)
{
	shouldDrawBrush = true;
}

ZGIImage::~ZGIImage()
{
}

void ZGIImage::SetImage(const char * imagePath)
{
	TextureBase* image = TextureManager::GetInstance()->TextureForFilePath(imagePath, Render_Data_Type_RGBA_32);

	widgetBrush->SetBackgroudImage(image);
	SetBounds(image->GetSize());
}

void ZGIImage::SetImage(TextureBase * image)
{
	widgetBrush->SetBackgroudImage(image);

	SetBounds(image->GetSize());
}

void ZGIImage::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Some Ancher stuff
}
