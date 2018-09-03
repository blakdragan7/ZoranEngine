#pragma once
#include <Core\2D\Components\Collision2DComponent.h>

class CircleCollision2DObject;
class CircleCollisionComponent : public Collision2DComponent
{
private:
	CircleCollision2DObject * circleCollision;

public:
	CircleCollisionComponent(Component2DBase* affectedComponent);
	~CircleCollisionComponent();

	void SetRadius(float radius);
	float GetRadius();
};

