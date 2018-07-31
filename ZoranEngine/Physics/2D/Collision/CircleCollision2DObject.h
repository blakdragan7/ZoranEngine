#pragma once
#include <Physics\2D\Collision\CollisionObject2DBase.h>

#define CIRCLE_COLLISION 10

class ZoranEngine_EXPORT CircleCollision2DObject : public CollisionObject2DBase
{
private:
	float radius;
	float scaledRadius;

public:
	CircleCollision2DObject(float radius,SceneObject2D* sceneObject,CollisionDynamics cd);
	~CircleCollision2DObject();

	void SetBoundsBySceneObject()override;

	virtual Vector2D GetSize()override;
	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other)override;
	virtual bool CollidesWith(CollisionObject2DBase* other, Collision2D* response)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;
};
