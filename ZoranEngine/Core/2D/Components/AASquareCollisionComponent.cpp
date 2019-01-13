#include "stdafx.h"
#include "AASquareCollisionComponent.h"
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>

AASquareCollisionComponent::AASquareCollisionComponent(Component2DBase* affectedComponent)
{
	squareCollision = new AABBSquareCollisionObject(affectedComponent);
	SetCollision(squareCollision);
}


AASquareCollisionComponent::~AASquareCollisionComponent()
{
}
