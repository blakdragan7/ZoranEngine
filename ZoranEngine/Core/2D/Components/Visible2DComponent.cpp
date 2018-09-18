#include "stdafx.h"
#include "Visible2DComponent.h"
#include <Rendering/RenderedObjectBase.h>

Visible2DComponent::Visible2DComponent(unsigned renderLayer) : VisibleComponentBase(renderLayer)
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
	renderedObject->RenderObject();
}

void Visible2DComponent::PostRender()
{
}
