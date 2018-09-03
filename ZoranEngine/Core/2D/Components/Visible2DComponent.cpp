#include "stdafx.h"
#include "Visible2DComponent.h"


Visible2DComponent::Visible2DComponent() : Component2DBase(CT_Visible)
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
