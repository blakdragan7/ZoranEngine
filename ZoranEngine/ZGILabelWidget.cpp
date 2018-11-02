#include "stdafx.h"
#include "ZGILabelWidget.h"

#include <Rendering/RenderEngineBase.h>
#include <Rendering/Renderers/FontRenderer.h>

ZGILabelWidget::ZGILabelWidget(ZGIVirtualWindow* owningWindow) : ZGIWidget(owningWindow)
{
	renderer = rEngine->CreateFontRenderer();
}

ZGILabelWidget::~ZGILabelWidget()
{
}

void ZGILabelWidget::SetText(const std::string & text)
{
}

void ZGILabelWidget::Render(const Matrix44 & projection)
{
	renderer->RenderObject(projection);
}
