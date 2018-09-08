#include "stdafx.h"
#include "RigidBody3DComponent.h"

#include <Physics/3D/PhysicsObject3DBase.h>

RigidBody3DComponent::RigidBody3DComponent(PhysicsObject3DBase * physics3D) : physics3D(physics3D), RigidBodyComponent(physics3D)
{

}

RigidBody3DComponent::~RigidBody3DComponent()
{
}

void RigidBody3DComponent::ApplyForce(const Vector3D& Force)
{
	physics3D->ApplyForce(Force);
}

void RigidBody3DComponent::SetVeloctiy(const Vector3D& Velocity)
{
	physics3D->SetVeloctiy(Velocity);
}

void RigidBody3DComponent::SetGravity(const Vector3D& gravity)
{
	physics3D->SetGravity(gravity);
}

const Vector3D& RigidBody3DComponent::GetVelocity()
{
	return physics3D->GetVelocity();
}
