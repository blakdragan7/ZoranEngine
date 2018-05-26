#pragma once
#include "CollisionObjectBase.h"

#define BOX_COLLISION 2

class DRAGENGINE_EXPORT BoxCollisionObject : public CollisionObjectBase
{
private:
	Vector3D minPos;
	Vector3D maxPos;

	Vector3D size;
	Vector3D scaledSize;

public:
	BoxCollisionObject(Vector3D min,Vector3D max,SceneObject* object, unsigned collisionType = BOX_COLLISION);
	~BoxCollisionObject();

	void SetBoundsBySceneObject();

	virtual bool CollidesWith(Vector3D pos)override;
	virtual bool CollidesWith(CollisionObjectBase* other)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;
	virtual Vector3D GetSize();
};

