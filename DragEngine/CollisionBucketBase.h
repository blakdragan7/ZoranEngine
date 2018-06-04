#pragma once
#include "Vector3.h"
#include <vector>
#include <mutex>

class CollisionObjectBase;
class BoxCollisionObject;
class SceneObject;
class PhysicsObjectBase;

// struct passed around that represents a collision between two points
struct DRAGENGINE_EXPORT CollisionResponse
{
	bool collided;
	Vector3D normal;
	Vector3D point;
	Vector3D intersection;
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
};

class DRAGENGINE_EXPORT CollisionBucketBase
{
protected:
	SceneObject * sceneObject;
	BoxCollisionObject* collision;

	unsigned maxObjects;

#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<CollisionObjectBase*> collisionObjects;
	std::mutex mutex;
#pragma warning(pop)

protected:
	int FindObject(CollisionObjectBase* object);

public:
	CollisionBucketBase(Vec3D pos, Vec3D size, unsigned maxObjects = 32);
	virtual ~CollisionBucketBase();

	virtual bool AddObject(CollisionObjectBase* object) = 0;
	virtual CollisionObjectBase* RemoveObject(CollisionObjectBase* object) = 0;

	virtual void CheckAllCollision() = 0;
	virtual bool ObjectIsWithinBucket(CollisionObjectBase* object) = 0;
	virtual bool CheckAllCollisionForObject(CollisionObjectBase* object, CollisionResponse& response) = 0;
};

