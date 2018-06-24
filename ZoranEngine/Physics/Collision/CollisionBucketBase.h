#pragma once
#include "Math/Vector3.h"
#include <vector>
#include <mutex>

class CollisionObjectBase;
class QuadTreeCollision;
class SceneObject;
class PhysicsObjectBase;

class ZoranEngine_EXPORT CollisionBucketBase
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
	virtual bool CheckObjectAgainstStaic(CollisionObjectBase* object, struct CollisionResponse& response) = 0;
	virtual bool CheckCollisionForObject(CollisionObjectBase* object, struct CollisionResponse& response) = 0;
	virtual bool SweepCollision(CollisionObjectBase* object, Vec3D newPosition, struct SweepCollisionResponse& response) = 0;
	virtual bool SweepCollisionHitTest(CollisionObjectBase* object, Vec3D newPosition, struct SweepCollisionResponse& response) = 0;
	// Debug Prints
	virtual void PrintAllCollisions() = 0;
	virtual void PrintAllContents(unsigned depth = 0) = 0;
};

