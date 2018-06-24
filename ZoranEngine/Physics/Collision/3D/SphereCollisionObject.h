#pragma once
#include "Physics/Collision/CollisionObjectBase.h"

#define SPHERE_COLLISION 1

class ZoranEngine_EXPORT SphereCollisionObject : public CollisionObjectBase
{
private:
	double radius;
	double radiusSqr;

	double scaledRadius;

public:
	SphereCollisionObject(double radius, SceneObject* object, CollisionDynamics dynamics,unsigned collisionType = SPHERE_COLLISION);
	~SphereCollisionObject();

	virtual void SetBoundsBySceneObject()override;

	virtual bool CollidesWith(CollisionObjectBase* other, CollisionResponse& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;

	virtual Vector3D GetSize()override;
	inline double GetRadius() { return radius; }
	inline double GetRadiusSqr() { return radiusSqr; }
};

