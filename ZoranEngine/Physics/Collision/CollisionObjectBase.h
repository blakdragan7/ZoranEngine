#pragma once
#include "Core/PlatformTypes.h"
#include "Math/Vector3.h"

#define NO_COLLISION 0

enum CollisionDynamics
{
	CD_Static=0,
	CD_Dynamic=1
};

struct ZoranEngine_EXPORT CollisionResponse
{
	bool collided;
	Vector3D normal;
	Vector3D point;
	Vector3D penetration;
	class PhysicsObjectBase* collidedObjects[2];
	class CollisionObjectBase* objectBounds[2];

	CollisionResponse()
	{
		collided = false;
		collidedObjects[0] = 0;
		collidedObjects[1] = 0;
		objectBounds[0] = 0;
		objectBounds[1] = 0;
	}

	CollisionResponse Reflection()
	{
		CollisionResponse res = *this;
		res.penetration = -res.penetration;
		res.normal = -res.normal;
		res.collidedObjects[0] = collidedObjects[1];
		res.collidedObjects[1] = collidedObjects[0];
		res.objectBounds[0] = objectBounds[1];
		res.objectBounds[1] = objectBounds[0];
		return res;
	};
};

struct ZoranEngine_EXPORT SweepCollisionResponse
{
	CollisionResponse collisionResponse;
	double timeHit;

	SweepCollisionResponse()
	{
		timeHit = 1.0;
	}
};

class SceneObject;
class PhysicsObjectBase;
class ZoranEngine_EXPORT CollisionObjectBase
{
private:
	unsigned collisionType;
	unsigned collisionLayer;
	SceneObject* sceneObject;
	PhysicsObjectBase* physicsObject;
	CollisionDynamics collisionDynamics;
	
public:
	bool isDirty;

public:
	CollisionObjectBase(SceneObject* object, CollisionDynamics collisionDynamics = CD_Dynamic, unsigned collisionType = NO_COLLISION);
	virtual ~CollisionObjectBase();

	void SetSceneObject(SceneObject* object);
	void SetPhysicsObject(PhysicsObjectBase* object);
	virtual void SetBoundsBySceneObject() = 0;

	inline unsigned GetCollisionType() { return collisionType; }
	virtual unsigned GetCollisionLayer();
	void SetCollisionLayer(unsigned layer);

	virtual bool CollidesWith(CollisionObjectBase* other, CollisionResponse& response) = 0;
	virtual Vector3D GetClosestPointTo(Vector3D pos) = 0;
	virtual Vector3D GetSize() = 0;

	void SetDynamics(CollisionDynamics dynamics);

	CollisionDynamics GetDynamics();
	Vector3D GetScenePos();
	SceneObject* GetSceneObject();
	PhysicsObjectBase* GetPhysicsObject();
};
