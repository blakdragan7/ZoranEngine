#pragma once
#include <Physics\2D\Collision\CollisionObject2DBase.h>

#define b2D_2D_COLLISION 7

class Component2DBase;
class ZoranEngine_EXPORT b2DCollision2DObject : public CollisionObject2DBase
{
public:
	b2DCollision2DObject(Component2DBase *component);
	~b2DCollision2DObject();

	void SetBoundsBySceneObject()override;

	virtual Vector2D GetSize()override;
	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other)override;
	virtual Collision2D* CollidesWith(CollisionObject2DBase* other)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;
};

