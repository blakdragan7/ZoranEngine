#include "stdafx.h"
#include "CollisionObject2DBase.h"
#include <Core/2D/SceneObject2D.h>

unsigned long long CollisionResponse2D::sID = 0;

CollisionObject2DBase::CollisionObject2DBase(SceneObject2D *object, CollisionDynamics collisionDynamics, unsigned collisionType) : CollisionObjectBase(object, collisionDynamics, collisionType)
{
	physicsObject2D = 0;
	sceneObject2D = object;
}

CollisionObject2DBase::~CollisionObject2DBase()
{
}

void CollisionObject2DBase::SetSceneObject(SceneObject2D* object)
{
	sceneObject2D = object;
}

void CollisionObject2DBase::SetPhysicsObject(PhysicsObject2DBase* object)
{
	physicsObject2D = object;
}

Vector2D CollisionObject2DBase::GetScenePos()
{
	if (sceneObject2D != NULL) return sceneObject2D->GetPosition();
	else
	{
		std::cerr << "Getting Position Before SceneObject Set !\n";
		return Vector2D(0, 0);
	}
}

SceneObject2D* CollisionObject2DBase::GetSceneObject() { return sceneObject2D; }
PhysicsObject2DBase* CollisionObject2DBase::GetPhysicsObject() { return physicsObject2D; }
