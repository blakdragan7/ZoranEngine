#include "stdafx.h"
#include "CollisionComponentBase.h"
#include "RigidBodyComponent.h"

#include <Physics/Collision/CollisionObjectBase.h>

CollisionComponentBase::CollisionComponentBase(CollisionObjectBase * collision) : collision(collision)
{
}

CollisionComponentBase::~CollisionComponentBase()
{
	delete collision;
}

void CollisionComponentBase::SetPhysicsObjectFromRigidBody(RigidBodyComponent * object)
{
	collision->SetPhysicsObject(object->GetPhyicsObject());
}

unsigned CollisionComponentBase::GetCollisionLayer()
{
	return collision->GetCollisionLayer();
}

void CollisionComponentBase::SetCollisionLayer(unsigned layer)
{
	collision->SetCollisionLayer(layer);
}

void CollisionComponentBase::SetDynamics(CollisionDynamics dynamics)
{
	collision->SetDynamics(dynamics);
}