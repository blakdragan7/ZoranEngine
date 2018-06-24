#pragma once
#include "Physics/Collision/CollisionObjectBase.h"

#define BOX_COLLISION 2

class ZoranEngine_EXPORT BoxCollisionObject : public CollisionObjectBase
{
private:
	Vector3D minPos;
	Vector3D maxPos;

	Vector3D size;
	Vector3D scaledSize;

public:
	BoxCollisionObject(Vector3D min,Vector3D max,SceneObject* object, CollisionDynamics dynamics = CD_Dynamic, unsigned collisionType = BOX_COLLISION);
	~BoxCollisionObject();

	void SetBoundsBySceneObject()override;

	inline Vec3D GetMinPos() { return minPos; }
	inline Vec3D GetMaxPos() { return maxPos; }

	virtual Vector3D GetSize()override;
	virtual bool CollidesWith(CollisionObjectBase* other, CollisionResponse& response)override;
	virtual Vector3D GetClosestPointTo(Vector3D pos)override;

	friend class PhysicsObjectBase;
};
