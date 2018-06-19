#pragma once
#include "CollisionObjectBase.h"

#define QUADTREE_COLLISION 3

class QuadTreeCollision : public CollisionObjectBase
{
private:
	Vector3D minPos;
	Vector3D maxPos;

	Vector3D size;
	Vector3D scaledSize;

private:
	bool QuadTreeCollision::CollidesWith(Vector3D pos);

public:
	QuadTreeCollision(Vector3D min, Vector3D max, SceneObject* object);
	~QuadTreeCollision();

	void SetBoundsBySceneObject()override;

	virtual Vector3D GetSize()override;
	virtual bool CollidesWith(CollisionObjectBase* other, CollisionResponse& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;
};

