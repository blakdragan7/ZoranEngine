#pragma once
#include <Physics/2D/Collision/CollisionObject2DBase.h>

#include "Math/Vector2.h"

#define SQUARE_COLLISION 4

class SquareColisionObject : public CollisionObject2DBase
{
private:
	Vector2D minPos;
	Vector2D maxPos;

	Vector2D size;
	Vector2D scaledSize;

public:
	SquareColisionObject(Vector2D min, Vector2D max, SceneObject2D* object, CollisionDynamics dynamics = CD_Dynamic);
	~SquareColisionObject();

	void SetBoundsBySceneObject()override;

	inline Vec2D GetMinPos() { return minPos; }
	inline Vec2D GetMaxPos() { return maxPos; }

	virtual Vector2D GetSize()override;
	virtual bool CollidesWith(CollisionObject2DBase* other, CollisionResponse2D& response)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;

	friend class PhysicsObject2DBase;
};

