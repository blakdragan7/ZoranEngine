#pragma once
#include "Vector3.h"
#include <vector>
#include <mutex>

class CollisionObjectBase;
class BoxCollisionObject;
class SceneObject;

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

public:
	CollisionBucketBase(Vec3D pos, Vec3D size, unsigned maxObjects = 32);
	virtual ~CollisionBucketBase();

	virtual void AddObject(CollisionObjectBase* object) = 0;
	virtual CollisionObjectBase* RemoveObject(CollisionObjectBase* object) = 0;

	virtual void CheckAllCollision() = 0;
	virtual bool ObjectIsWithinBucket(CollisionObjectBase* object) = 0;
};

