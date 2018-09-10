#pragma once
#include <Core\2D\Components\Collision2DComponent.h>
class AABBSquareCollisionObject;
class ZoranEngine_EXPORT AASquareCollisionComponent : public Collision2DComponent
{
private:
	AABBSquareCollisionObject * squareCollision;

public:
	AASquareCollisionComponent(Component2DBase* affectedComponent);
	~AASquareCollisionComponent();
};

