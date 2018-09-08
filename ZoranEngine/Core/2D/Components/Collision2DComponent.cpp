#include "stdafx.h"
#include "Collision2DComponent.h"

#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include <Core/2D/Components/RigidBody2DComponent.h>

Collision2DComponent::Collision2DComponent() : preCast(0)
{
	
}

Collision2DComponent::Collision2DComponent(CollisionObject2DBase* collision) : preCast(collision), CollisionComponentBase(collision)
{
}


Collision2DComponent::~Collision2DComponent()
{
}

void Collision2DComponent::SetPhysicsObjectFrom2DRigidBody(RigidBody2DComponent * object)
{
	if (preCast)
	{
		SetPhysicsObjectFromRigidBody(object);
		preCast->SetPhysicsObject(object->GetPhysics2D());
	}
	else
	{
		Log(LogLevel_Error,"Collision2DComponent::SetPhysicsObjectFrom2DRigidBody Called without a Collision Object Set !\n");
	}
}
