#pragma once
#include "Vector2.h"
#include <vector>
#include <mutex>

class CollisionObjectBase;
class DRAGENGINE_EXPORT QuadTreeCollisionBucket
{
private:
	QuadTreeCollisionBucket * parent;
	QuadTreeCollisionBucket* children[4];

	Vec2L pos;
	Vec2L size;

	Vec2L min;
	Vec2L max;

	unsigned maxObjects;

#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<CollisionObjectBase*> collisionObjects;
	std::mutex mutex;
#pragma warning(pop)

public:
	QuadTreeCollisionBucket(Vec2L pos,Vec2L size,unsigned maxObjects = 32,QuadTreeCollisionBucket * parent=0);
	~QuadTreeCollisionBucket();

	void AddObject(CollisionObjectBase* object);
	CollisionObjectBase* RemoveObject(CollisionObjectBase* object);

	void Subdivide();
};

