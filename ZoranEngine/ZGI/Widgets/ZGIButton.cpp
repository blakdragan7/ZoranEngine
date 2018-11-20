#include "stdafx.h"
#include "ZGIButton.h"

#include <Rendering/Renderers/GUIRenderer.h>
#include <Rendering/RenderEngineBase.h>

ZGIButton::ZGIButton(ZGIVirtualWindow* owningWindow) : ZGIWidget(owningWindow)
{
	renderer = rEngine->CreateGUIRenderer();
	renderer->SetHasTexture(false);
	renderer->SetTint({ 1.0f,1.0f,1.0f,1.0f });
}

ZGIButton::~ZGIButton()
{
	delete renderer;
}

void ZGIButton::Render(const Matrix44 & projection)
{
	renderer->RenderObject(projection * modelCache);
}

void ZGIButton::ContainerResized(Vec2D newSize, Vec2D oldSize)
{
	// TODO: Ancher Stuff
}

bool ZGIButton::MouseDown(const PlatformMouseBase * mouse)
{
	renderer->SetTint({ 0.3f,0.3f,0.3f,1.0f });
	return false;
}

bool ZGIButton::MouseUp(const PlatformMouseBase * mouse)
{
	renderer->SetTint({ 0.7f,0.7f,0.7f,1.0f });
	return false;
}

bool ZGIButton::MouseEnterd(const PlatformMouseBase * mouse)
{
	renderer->SetTint({ 0.9f,0.9f,0.9f,1.0f });
	return false;
}

bool ZGIButton::MouseLeft(const PlatformMouseBase * mouse)
{
	renderer->SetTint({ 0.7f,0.7f,0.7f,1.0f });
	return false;
}
