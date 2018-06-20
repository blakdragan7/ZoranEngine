#pragma once
#include "CollisionObjectBase.h"

#include "Vector2.h"

#define SQUARE_COLLISION 4

class SquareColisionObject : public CollisionObjectBase
{
private:
	Vector2D minPos;
	Vector2D maxPos;

	Vector2D size;
	Vector2D scaledSize;

public:
	SquareColisionObject(Vector2D min, Vector2D max, SceneObject* object, CollisionDynamics dynamics = CD_Dynamic, unsigned collisionType = SQUARE_COLLISION);
	~SquareColisionObject();

	void SetBoundsBySceneObject()override;

	inline Vec2D GetMinPos() { return minPos; }
	inline Vec2D GetMaxPos() { return maxPos; }

	virtual Vector3D GetSize()override;
	virtual bool CollidesWith(CollisionObjectBase* other, CollisionResponse& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;

	friend class PhysicsObjectBase;
};

