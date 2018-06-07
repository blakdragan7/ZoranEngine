#pragma once

#include "CollisionBucketBase.h"

#define TL_BUCKET 0
#define TR_BUCKET 1
#define BL_BUCKET 2
#define BR_BUCKET 3


class DRAGENGINE_EXPORT QuadTreeCollisionBucket : public CollisionBucketBase
{
private:
	QuadTreeCollisionBucket * parent;
	QuadTreeCollisionBucket* children[4];

	bool hasSubdivided;

private:
	void Subdivide();
	bool CheckCollisionForObject(CollisionObjectBase* object, CollisionResponse& response);

public:
	QuadTreeCollisionBucket(std::string name, Vec3D pos,Vec3D size,unsigned maxObjects = 2,QuadTreeCollisionBucket * parent=0);
	~QuadTreeCollisionBucket();

	bool AddObject(CollisionObjectBase* object)override;
	CollisionObjectBase* RemoveObject(CollisionObjectBase* object)override;

	virtual bool UpdateObject(CollisionObjectBase* object)override;

	virtual void UpdateAllObjects()override;
	void CheckAllCollision()override;
	bool ObjectIsWithinBucket(CollisionObjectBase* object)override;
	virtual bool CheckAllCollisionForObject(CollisionObjectBase* object, CollisionResponse& response)override;
};

