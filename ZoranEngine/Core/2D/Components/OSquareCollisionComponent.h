#pragma once
#include <Core\2D\Components\Collision2DComponent.h>

class Component2DBase;
class OSquareCollisionComponent : public Collision2DComponent
{
private:
	b2DCollision2DObject * squareCollision;

public:
	OSquareCollisionComponent(Component2DBase* affectedComponent);
	~OSquareCollisionComponent();
};

