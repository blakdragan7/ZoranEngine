#include "stdafx.h"
#include "OSquareCollisionComponent.h"

#include <Physics/2D/Collision/b2DCollision2DObject.h>

OSquareCollisionComponent::OSquareCollisionComponent(Component2DBase* affectedComponent) :
	squareCollision(new b2DCollision2DObject(affectedComponent)), Collision2DComponent(squareCollision)
{
}

OSquareCollisionComponent::~OSquareCollisionComponent()
{
}
