#include "stdafx.h"
#include "Collision3DComponent.h"

#include <Physics/3D/Collision/CollisionObject3DBase.h>

Collision3DComponent::Collision3DComponent(CollisionObject3DBase * collision) : preCast(collision), 
CollisionComponentBase(collision) , Component3DBase(CT_Collision)
{
}


Collision3DComponent::~Collision3DComponent()
{
}
