#include "stdafx.h"
#include "Visible3DComponent.h"


Visible3DComponent::Visible3DComponent() : Component3DBase(CT_Visible)
{
}


Visible3DComponent::~Visible3DComponent()
{
}

void Visible3DComponent::PreRender()
{
	if (isDirty)
	{
		CalculateLocalMatrix();
		isDirty = false;
	}
}