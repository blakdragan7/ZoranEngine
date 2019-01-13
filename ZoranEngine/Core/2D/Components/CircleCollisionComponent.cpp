#include "stdafx.h"
#include "CircleCollisionComponent.h"

#include <Physics/2D/Collision/CircleCollision2DObject.h>

CircleCollisionComponent::CircleCollisionComponent(Component2DBase* affectedComponent) :
	circleCollision(new CircleCollision2DObject(0, affectedComponent))
{
	SetCollision(circleCollision);
}

CircleCollisionComponent::~CircleCollisionComponent()
{
}

void CircleCollisionComponent::SetRadius(float radius)
{
	circleCollision->SetRadius(radius);
}

float CircleCollisionComponent::GetRadius()
{
	return circleCollision->GetRadius();
}
