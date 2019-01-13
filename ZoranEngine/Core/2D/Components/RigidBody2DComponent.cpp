#include "stdafx.h"
#include "RigidBody2DComponent.h"


#include <Physics/2D/PhysicsObject2DBase.h>

RigidBody2DComponent::RigidBody2DComponent(Component2DBase* affectComponent) : physics2D(new PhysicsObject2DBase(affectComponent))
{
	SetPhysicsObject(physics2D);
}

RigidBody2DComponent::~RigidBody2DComponent()
{
}

void RigidBody2DComponent::ApplyForce(const Vector2D& force)
{
	physics2D->ApplyForce(force);
}

void RigidBody2DComponent::SetVeloctiy(const Vector2D& velocity)
{
	physics2D->SetVeloctiy(velocity);
}

void RigidBody2DComponent::SetGravity(const Vector2D& gravity)
{
	physics2D->SetGravity(gravity);
}

void RigidBody2DComponent::SetAngularVeloctiy(float Velocity)
{
	physics2D->SetAngularVeloctiy(Velocity);
}

void RigidBody2DComponent::ApplyImpulseToVelocity(const Vector2D& impulse)
{
	physics2D->ApplyImpulseToVelocity(impulse);
}

void RigidBody2DComponent::ApplyImpulseToAngularVelocity(float impulse)
{
	physics2D->ApplyImpulseToAngularVelocity(impulse);
}

float RigidBody2DComponent::GetAngularVelocity()
{
	return physics2D->GetAngularVelocity();
}

Vector2D RigidBody2DComponent::GetVelocity()
{
	return physics2D->GetVelocity();
}
