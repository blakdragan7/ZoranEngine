#pragma once
#include "Vector3.h"
#include <vector>
#include <mutex>

class CollisionObjectBase;
class QuadTreeCollision;
class SceneObject;
class PhysicsObjectBase;

// struct passed around that represents a collision between two points
struct DRAGENGINE_EXPORT CollisionResponse
{
	bool collided;
	Vector3D normal;
	Vector3D point;
	Vector3D penetration;
	PhysicsObjectBase* collidedObjects[2];
	CollisionObjectBase* objectBounds[2];

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
		res.normal = -res.normal;
		res.collidedObjects[0] = collidedObjects[1];
		res.collidedObjects[1] = collidedObjects[0];
		res.objectBounds[0] = objectBounds[1];
		res.objectBounds[1] = objectBounds[0];
		return res;
	};
};

struct SweepCollisionResponse
{
	CollisionResponse collisionResponse;
	double timeHit;

	SweepCollisionResponse()
	{
		timeHit = 1.0;
	}
};

class DRAGENGINE_EXPORT CollisionBucketBase
{
protected:
	SceneObject * sceneObject;
	QuadTreeCollision* collision;

	unsigned maxObjects;

#pragma warning(push)
#pragma warning(disable:4251)
	// this should be split into dynamic / static objects for effeicinecy.
	// It's a lot of work though so unless we need it at some point, it will be left like this for now
	std::vector<CollisionObjectBase*> collisionObjects;
	std::mutex mutex;
#pragma warning(pop)

protected:
	int FindObject(CollisionObjectBase* object);

public:
	CollisionBucketBase(std::string name, Vec3D pos, Vec3D size, unsigned maxObjects = 32);
	virtual ~CollisionBucketBase();

	virtual bool AddObject(CollisionObjectBase* object) = 0;
	virtual CollisionObjectBase* RemoveObject(CollisionObjectBase* object) = 0;

	virtual void UpdateAllObjects() = 0;
	virtual bool UpdateObject(CollisionObjectBase* object) = 0;

	virtual void CheckAllCollision() = 0;
	virtual bool ObjectIsWithinBucket(CollisionObjectBase* object) = 0;
	virtual bool CheckObjectAgainstStaic(CollisionObjectBase* object, CollisionResponse& response) = 0;
	virtual bool CheckCollisionForObject(CollisionObjectBase* object, CollisionResponse& response) = 0;
	virtual bool SweepCollision(CollisionObjectBase* object, Vec3D newPosition, SweepCollisionResponse& response) = 0;
	virtual bool SweepCollisionHitTest(CollisionObjectBase* object, Vec3D newPosition, SweepCollisionResponse& response) = 0;
	// Debug Prints
	virtual void PrintAllCollisions() = 0;
	virtual void PrintAllContents(unsigned depth = 0) = 0;
};

