#pragma once
#include "Vector3.h"
#include <vector>
#include <mutex>

#define TL_BUCKET 0
#define TR_BUCKET 0
#define BL_BUCKET 0
#define BR_BUCKET 0

class CollisionObjectBase;
class BoxCollisionObject;
class SceneObject;
class DRAGENGINE_EXPORT QuadTreeCollisionBucket
{
private:
	QuadTreeCollisionBucket * parent;
	QuadTreeCollisionBucket* children[4];

	bool hasSubdivided;

	SceneObject* sceneObject;
	BoxCollisionObject* collision;

	unsigned maxObjects;

#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<CollisionObjectBase*> collisionObjects;
	std::mutex mutex;
#pragma warning(pop)

private:
	void Subdivide();

public:
	QuadTreeCollisionBucket(Vec3D pos,Vec3D size,unsigned maxObjects = 32,QuadTreeCollisionBucket * parent=0);
	~QuadTreeCollisionBucket();

	void AddObject(CollisionObjectBase* object);
	CollisionObjectBase* RemoveObject(CollisionObjectBase* object);

	void CheckAllCollision();
	bool ObjectIsWithinBucket(CollisionObjectBase* object);
};

