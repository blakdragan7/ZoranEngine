#pragma once
#include "CollisionObjectBase.h"

#define SPHERE_COLLISION 1

class SphereCollisionObject : public CollisionObjectBase
{
private:
	double radius;
	double radiusSqr;

public:
	SphereCollisionObject(double radius, SceneObject* object, unsigned collisionType = SPHERE_COLLISION);
	~SphereCollisionObject();

	virtual bool CollidesWith(Vector3D pos)override;
	virtual bool CollidesWith(CollisionObjectBase* other)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;
};

