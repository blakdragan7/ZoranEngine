#include "stdafx.h"
#include "Visible2DComponent.h"
#include <Rendering/RenderedObjectBase.h>

Visible2DComponent::Visible2DComponent(unsigned renderLayer) : shouldRender(true), VisibleComponentBase(renderLayer)
{
}


Visible2DComponent::~Visible2DComponent()
{
}

void Visible2DComponent::PreRender()
{
	if (isDirty)
	{
		CalculateLocalMatrix();
		isDirty = false;
	}
}

void Visible2DComponent::Render()
{
	if(shouldRender)
		renderedObject->RenderObject();
}

void Visible2DComponent::PostRender()
{
}
