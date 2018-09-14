#include "stdafx.h"
#include "Speaker2DComponent.h"

#include <Core/2D/Components/RigidBody2DComponent.h>

Speaker2DComponent::Speaker2DComponent(RigidBody2DComponent* physics) : physics(physics)
{
}


Speaker2DComponent::~Speaker2DComponent()
{
}

void Speaker2DComponent::Tick(float dt)
{
	// TODO optimize this to not be every frame

	Vector2D worldLocation = GetWorldLocation();
	SetPosisition(worldLocation.x, worldLocation.y);
	
	float rotation = GetRotation();

	/* 2D so convert rotation to up vector and assume forward is alwaus +z */
	SetOrientation(cosf(rotation),sinf(rotation),0,0,0,1);

	if (physics)
	{
		Vector2D vel = physics->GetVelocity();
		SetVelocity(vel.x,vel.y);
	}
}
