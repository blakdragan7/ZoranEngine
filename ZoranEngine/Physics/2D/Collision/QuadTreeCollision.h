#pragma once
#include "CollisionObject2DBase.h"
#define QUADTREE_COLLISION 3

class QuadTreeCollision : public CollisionObject2DBase
{
private:
	Vector2D minPos;
	Vector2D maxPos;
		  
	Vector2D size;
	Vector2D scaledSize;

private:
	bool QuadTreeCollision::CollidesWith(Vector3D pos);

public:
	QuadTreeCollision(Vector2D min, Vector2D max, SceneObject2D* object);
	~QuadTreeCollision();

	void SetBoundsBySceneObject()override;

	virtual Vector2D GetSize()override;
	virtual bool CollidesWith(CollisionObject2DBase* other, Collision2D& response)override;
	virtual Vector2D GetClosestPointTo(Vector2D pos)override;
};

