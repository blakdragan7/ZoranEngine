#pragma once
#include "CollisionObjectBase.h"

#define SPHERE_COLLISION 1

class DRAGENGINE_EXPORT SphereCollisionObject : public CollisionObjectBase
{
private:
	double radius;
	double radiusSqr;

	double scaledRadius;

public:
	SphereCollisionObject(double radius, SceneObject* object, CollisionDynamics dynamics,unsigned collisionType = SPHERE_COLLISION);
	~SphereCollisionObject();

	virtual void SetBoundsBySceneObject()override;

	virtual bool CollidesWith(Vector3D pos)override;
	virtual bool CollidesWith(CollisionObjectBase* other)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;
	virtual Vector3D GetNormalBetween(CollisionObjectBase* other)override;

	virtual Vector3D GetSize()override;
	inline double GetRadius() { return radius; }
	inline double GetRadiusSqr() { return radiusSqr; }
};

