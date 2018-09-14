#include "stdafx.h"
#include "Speaker3DComponent.h"

#include <Core/3D/Components/RigidBody3DComponent.h>
Speaker3DComponent::Speaker3DComponent(RigidBody3DComponent * physics) : physics(physics)
{
}


Speaker3DComponent::~Speaker3DComponent()
{
}

void Speaker3DComponent::Tick(float dt)
{
	// TODO optimize this to not be every frame

	Vector3D worldLocation = GetWorldLocation();
	SetPosisition(worldLocation.x, worldLocation.y, worldLocation.z);

	Quaternion orientation = GetRotation();

	Vec3D up = orientation.GetUpVector();
	Vec3D forward = orientation.GetForwardVector();

	SetOrientation(up.x, up.y, up.z, forward.x, forward.y, forward.z);

	if (physics)
	{
		Vector3D vel = physics->GetVelocity();
		SetVelocity(vel.x, vel.y, vel.z);
	}
}
