#pragma once
#include "PhysicsObjectBase.h"

class BoxCollisionObject : public PhysicsObjectBase
{
private:
	Vector3D minPos;
	Vector3D maxPos;

public:
	BoxCollisionObject(Vector3D min,Vector3D max,SceneObject* object);
	~BoxCollisionObject();

	virtual bool CollidesWith(Vector3D pos);
	virtual bool CollidesWith(PhysicsObjectBase* other);
	virtual Vector3D GetClosestPointTo(Vector3D pos);
};

