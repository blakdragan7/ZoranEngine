#pragma once
#include <Physics/3D/Collision/CollisionObject3DBase.h>

#define AABBOX_COLLISION 2

class ZoranEngine_EXPORT AABBoxCollisionObject : public CollisionObject3DBase
{
private:
	Vector3D minPos;
	Vector3D maxPos;

	Vector3D size;
	Vector3D scaledSize;

public:
	AABBoxCollisionObject(Vector3D min,Vector3D max,SceneObject3D* object, CollisionDynamics dynamics = CD_Dynamic, unsigned collisionType = AABBOX_COLLISION);
	~AABBoxCollisionObject();

	void SetBoundsBySceneObject()override;

	inline Vec3D GetMinPos() { return minPos; }
	inline Vec3D GetMaxPos() { return maxPos; }

	virtual Vector3D GetSize()override;
	virtual bool CollidesWith(CollisionObject3DBase* other, CollisionResponse3D& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;

	friend class PhysicsObject3DBase;
};

