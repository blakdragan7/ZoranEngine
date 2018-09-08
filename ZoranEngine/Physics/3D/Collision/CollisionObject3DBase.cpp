#include "stdafx.h"
#include "CollisionObject3DBase.h"
#include <Core/3D/Components/Component3DBase.h>

#include <iostream>

CollisionObject3DBase::CollisionObject3DBase(Component3DBase* compomnent, CollisionDynamics collisionDynamics, unsigned collisionType) : 
	physicsObject3D(0), affectedComponent(compomnent), CollisionObjectBase(collisionDynamics,collisionType)
{
}

CollisionObject3DBase::~CollisionObject3DBase()
{
}

void CollisionObject3DBase::SetAffectedComponent(Component3DBase* compomnent)
{
	affectedComponent = compomnent;
}

void CollisionObject3DBase::SetPhysicsObject(PhysicsObject3DBase* object)
{
	physicsObject3D = object;
}

Vector3D CollisionObject3DBase::GetScenePos()
{
	if (affectedComponent != NULL) return affectedComponent->GetWorldLocation();
	else
	{
		std::cerr << "Getting Position Before SceneObject Set !\n";
		return Vector3D(0, 0, 0);
	}
}

Component3DBase* CollisionObject3DBase::GetAffectedComponent() { return affectedComponent; }
PhysicsObject3DBase* CollisionObject3DBase::GetPhysicsObject() { return physicsObject3D; }
