#include "stdafx.h"
#include "Collision3DComponent.h"

#include <Physics/3D/Collision/CollisionObject3DBase.h>

Collision3DComponent::Collision3DComponent(CollisionObject3DBase * collision) : preCast(collision)
{
	SetCollisionObject(collision);
}


Collision3DComponent::~Collision3DComponent()
{
}
