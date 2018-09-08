#include "stdafx.h"
#include "CollisionComponentBase.h"
#include "RigidBodyComponent.h"

#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/PhysicsEngine.h>

CollisionComponentBase::CollisionComponentBase() : collision(0)
{
}

CollisionComponentBase::CollisionComponentBase(CollisionObjectBase * collision) : collision(collision)
{
	pEngine->AddCollisionObject(this->collision);
}

CollisionComponentBase::~CollisionComponentBase()
{
	pEngine->RemoveObject(collision);
	delete collision;
}

void CollisionComponentBase::SetCollisionObject(CollisionObjectBase* object)
{
	if (collision)
	{
		pEngine->RemoveObject(collision);
		delete collision;
	}

	collision = object;
	pEngine->AddCollisionObject(this->collision);
}

void CollisionComponentBase::SetPhysicsObjectFromRigidBody(RigidBodyComponent * object)
{
	collision->SetPhysicsObject(object->GetPhyicsObject());
}

unsigned CollisionComponentBase::GetCollisionLayer()const
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