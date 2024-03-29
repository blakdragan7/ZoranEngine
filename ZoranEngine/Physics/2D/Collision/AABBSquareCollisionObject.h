#pragma once
#include <Physics/2D/Collision/CollisionObject2DBase.h>

#include <Math/Vector2.h>

#define SQUARE_COLLISION 4

class ZoranEngine_EXPORT AABBSquareCollisionObject : public CollisionObject2DBase
{
private:
	Vector2D minPos;
	Vector2D maxPos;

	Vector2D size;
	Vector2D scaledSize;

	AABBSquareCollisionObject* sweepCollisionSquare;

private:
	void FillCollisionPoints(CollisionPoint points[2], Vec2D otherMin, Vec2D otherMax);

public:
	AABBSquareCollisionObject(Component2DBase* affectedComponent, CollisionDynamics dynamics = CD_Dynamic, bool isRoot = true);
	AABBSquareCollisionObject(const Vector2D& min, const Vector2D& max, Component2DBase* affectedComponent, CollisionDynamics dynamics = CD_Dynamic,bool isRoot = true);
	~AABBSquareCollisionObject();

	void SetBoundsBySceneObject()override;

	inline Vec2D GetMinPos() { return minPos; }
	inline Vec2D GetMaxPos() { return maxPos; }

	virtual Vector2D GetSize()override;
	virtual bool CollidesWithNoCollision(CollisionObject2DBase* other)override;
	virtual  Collision2D* CollidesWith(CollisionObject2DBase* other)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	virtual bool SweepCollidesWith(CollisionObject2DBase* other, Vector2D newPosition, SweepCollision2D & response)override;
	virtual bool FastSweepCollidesWith(Vector2D newPosition)override;

	friend class PhysicsObject2DBase;
};

