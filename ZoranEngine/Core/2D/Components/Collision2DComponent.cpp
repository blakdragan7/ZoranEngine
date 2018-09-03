#include "stdafx.h"
#include "Collision2DComponent.h"

#include <Physics/2D/Collision/CollisionObject2DBase.h>

Collision2DComponent::Collision2DComponent(CollisionObject2DBase* collision) : preCast(collision), CollisionComponentBase(collision), Component2DBase(CT_Collision)
{
}


Collision2DComponent::~Collision2DComponent()
{
}
