#pragma once
#include <Physics\2D\Collision\CollisionObject2DBase.h>

#define CIRCLE_COLLISION 10

class ZoranEngine_EXPORT CircleCollision2DObject : public CollisionObject2DBase
{
private:
	float radius;
	float scaledRadius;
	float scaledRadiusSqr;

public:
	CircleCollision2DObject(float radius, Component2DBase* component,CollisionDynamics cd = CD_Dynamic);
	~CircleCollision2DObject();

	void SetBoundsBySceneObject()override;

	inline float GetRadius() { return scaledRadius; }
	inline void SetRadius(float radius) { this->radius = radius; }

	virtual Vector2D GetSize()override;
	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other)override;
	virtual Collision2D* CollidesWith(CollisionObject2DBase* other)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;
};

