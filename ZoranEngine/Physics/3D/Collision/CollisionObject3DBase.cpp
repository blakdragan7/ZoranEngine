#include "stdafx.h"
#include "CollisionObject3DBase.h"
#include "Core/3D/SceneObject3D.h"

#include <iostream>

CollisionObject3DBase::CollisionObject3DBase(SceneObject3D* object, CollisionDynamics collisionDynamics, unsigned collisionType) : CollisionObjectBase(object, collisionDynamics,collisionType)
{
	physicsObject3D = 0;
	sceneObject3D = object;
}

CollisionObject3DBase::~CollisionObject3DBase()
{
}

void CollisionObject3DBase::SetSceneObject(SceneObject3D* object)
{
	sceneObject3D = object;
}

void CollisionObject3DBase::SetPhysicsObject(PhysicsObject3DBase* object)
{
	physicsObject3D = object;
}

Vector3D CollisionObject3DBase::GetScenePos()
{
	if (sceneObject3D != NULL) return sceneObject3D->GetPosition();
	else
	{
		std::cerr << "Getting Position Before SceneObject Set !\n";
		return Vector3D(0, 0, 0);
	}
}

SceneObject3D* CollisionObject3DBase::GetSceneObject() { return sceneObject3D; }
PhysicsObject3DBase* CollisionObject3DBase::GetPhysicsObject() { return physicsObject3D; }
