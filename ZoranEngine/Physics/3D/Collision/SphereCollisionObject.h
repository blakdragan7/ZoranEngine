#pragma once
#include <Physics/3D/Collision/CollisionObject3DBase.h>

#define SPHERE_COLLISION 1

class ZoranEngine_EXPORT SphereCollisionObject : public CollisionObject3DBase
{
private:
	double radius;
	double radiusSqr;

	double scaledRadius;

public:
	SphereCollisionObject(double radius, SceneObject3D* object, CollisionDynamics dynamics,unsigned collisionType = SPHERE_COLLISION);
	~SphereCollisionObject();

	virtual void SetBoundsBySceneObject()override;

	virtual bool CollidesWith(CollisionObject3DBase* other, CollisionResponse3D& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;

	virtual Vector3D GetSize()override;
	inline double GetRadius() { return radius; }
	inline double GetRadiusSqr() { return radiusSqr; }
};

