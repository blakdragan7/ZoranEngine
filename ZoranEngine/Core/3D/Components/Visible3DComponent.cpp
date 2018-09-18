#include "stdafx.h"
#include "Visible3DComponent.h"

#include <Rendering/3D/RenderEngine3DBase.h>

Visible3DComponent::Visible3DComponent(RenderEngine3DBase * engine3D) : engine3D(engine3D), VisibleComponentBase(engine3D)
{
	if (engine3D == 0)
		this->engine3D = r3Engine;
}


Visible3DComponent::~Visible3DComponent()
{
	if (program)
		engine3D->RemoveComponent(this);
}

void Visible3DComponent::PreRender()
{
	if (isDirty)
	{
		CalculateLocalMatrix();
		isDirty = false;
	}
}

void Visible3DComponent::SetShaderProgram(ShaderProgramBase * program)
{
	if (this->program)
	{
		engine3D->RemoveComponent(this);
	}

	this->program = program;
	engine3D->AddComponent(this);
}