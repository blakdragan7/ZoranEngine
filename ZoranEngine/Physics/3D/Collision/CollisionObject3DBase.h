#pragma once
#include <Physics/Collision/CollisionObjectBase.h>

class PhysicsObject3DBase;
class SceneObject3D;

struct ZoranEngine_EXPORT CollisionResponse3D
{
	bool collided;
	Vector3D normal;
	Vector3D point;
	Vector3D penetration;
	PhysicsObject3DBase* collidedObjects[2];
	class CollisionObject3DBase* objectBounds[2];

	CollisionResponse3D()
	{
		collided = false;
		collidedObjects[0] = 0;
		collidedObjects[1] = 0;
		objectBounds[0] = 0;
		objectBounds[1] = 0;
	}

	CollisionResponse3D Reflection()
	{
		CollisionResponse3D res = *this;
		res.penetration = -res.penetration;
		res.normal = -res.normal;
		res.collidedObjects[0] = collidedObjects[1];
		res.collidedObjects[1] = collidedObjects[0];
		res.objectBounds[0] = objectBounds[1];
		res.objectBounds[1] = objectBounds[0];
		return res;
	};
};

struct ZoranEngine_EXPORT SweepCollisionResponse3D
{
	CollisionResponse3D CollisionResponse3D;
	float timeHit;

	SweepCollisionResponse3D()
	{
		timeHit = 1.0;
	}
};

class ZoranEngine_EXPORT CollisionObject3DBase : public CollisionObjectBase
{
private:
	SceneObject3D* sceneObject3D;
	PhysicsObject3DBase* physicsObject3D;

public:
	CollisionObject3DBase(SceneObject3D* object, CollisionDynamics collisionDynamics = CD_Dynamic, unsigned collisionType = NO_COLLISION);
	virtual ~CollisionObject3DBase();

	void SetSceneObject(SceneObject3D* object);
	void SetPhysicsObject(PhysicsObject3DBase* object);

	virtual bool CollidesWith(CollisionObject3DBase* other, CollisionResponse3D& response) = 0;
	virtual Vector3D GetClosestPointTo(Vector3D pos) = 0;
	virtual Vector3D GetSize() = 0;


	Vector3D GetScenePos();
	SceneObject3D* GetSceneObject();
	PhysicsObject3DBase* GetPhysicsObject();
};

