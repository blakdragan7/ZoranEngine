#include "stdafx.h"
#include "RigidBodyComponent.h"

#include <Physics/PhysicsObjectBase.h>

RigidBodyComponent::RigidBodyComponent(PhysicsObjectBase* pObject) : physicsObject(pObject)
{
}

RigidBodyComponent::~RigidBodyComponent()
{
}

void RigidBodyComponent::StartSimulation()
{
	physicsObject->StartPhysicsSim();
}

void RigidBodyComponent::StopSimulation()
{
	physicsObject->StopPhysicsSim();
}

void RigidBodyComponent::SetMass(float mass)
{
	physicsObject->SetMass(mass);
}

void RigidBodyComponent::SetFriction(float friction)
{
	physicsObject->SetFriction(friction);
}

void RigidBodyComponent::SetRestitution(float restitution)
{
	physicsObject->SetRestitution(restitution);
}

void RigidBodyComponent::SetDrag(float drag)
{
	physicsObject->SetDrag(drag);
}

float RigidBodyComponent::GetFriction()
{
	return physicsObject->GetFriction();
}

float RigidBodyComponent::GetInvMass()
{

	return physicsObject->GetInvMass();
}

float RigidBodyComponent::GetMass()
{
	return physicsObject->GetMass();

}

float RigidBodyComponent::GetInvInertia()
{
	return physicsObject->GetInvInertia();

}

float RigidBodyComponent::GetInertia()
{
	return physicsObject->GetInertia();

}

bool RigidBodyComponent::GetIsSimulating()
{
	return physicsObject->GetShouldSimulate();
}
