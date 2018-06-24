#pragma once

#include "Physics/Collision/CollisionBucketBase.h"

#define TL_BUCKET 0
#define TR_BUCKET 1
#define BL_BUCKET 2
#define BR_BUCKET 3


class ZoranEngine_EXPORT QuadTreeCollisionBucket : public CollisionBucketBase
{
private:
	QuadTreeCollisionBucket * parent;
	QuadTreeCollisionBucket* children[4];

	bool hasSubdivided;

private:
	void Subdivide();
	bool PrintCollisionForObject(CollisionObjectBase * object);

	bool TraverseUpAddObject(CollisionObjectBase* object);

public:
	QuadTreeCollisionBucket(std::string name, Vec3D pos,Vec3D size,unsigned maxObjects = 128,QuadTreeCollisionBucket * parent=0);
	~QuadTreeCollisionBucket();

	bool AddObject(CollisionObjectBase* object)override;
	CollisionObjectBase* RemoveObject(CollisionObjectBase* object)override;

	virtual bool UpdateObject(CollisionObjectBase* object)override;

	virtual void PrintAllContents(unsigned depth=0)override;
	virtual void PrintAllCollisions()override;

	virtual void UpdateAllObjects()override;
	void CheckAllCollision()override;
	bool ObjectIsWithinBucket(CollisionObjectBase* object)override;
	bool CheckCollisionForObject(CollisionObjectBase* object, CollisionResponse& response)override;
	virtual bool CheckObjectAgainstStaic(CollisionObjectBase* object, CollisionResponse& response)override;
	virtual bool SweepCollision(CollisionObjectBase* object, Vec3D newPosition, struct SweepCollisionResponse& response)override;
	virtual bool SweepCollisionHitTest(CollisionObjectBase* object, Vec3D newPosition,struct SweepCollisionResponse& response)override;
};
