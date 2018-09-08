#include "stdafx.h"
#include "Visible2DComponent.h"


Visible2DComponent::Visible2DComponent()
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
}

void Visible2DComponent::PostRender()
{
}
