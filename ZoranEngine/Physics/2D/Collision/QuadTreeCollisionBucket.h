#pragma once

#include <Physics/2D/Collision/CollisionBucket2DBase.h>

#define TL_BUCKET 0
#define TR_BUCKET 1
#define BL_BUCKET 2
#define BR_BUCKET 3

class ZoranEngine_EXPORT QuadTreeCollisionBucket : public CollisionBucket2DBase
{
private:
	QuadTreeCollisionBucket * parent;
	QuadTreeCollisionBucket* children[4];

	bool hasSubdivided;

private:
	void Subdivide();
	bool PrintCollisionForObject(CollisionObject2DBase * object);

	bool TraverseUpAddObject(CollisionObject2DBase* object);

public:
	QuadTreeCollisionBucket(std::string name, Vec2D pos,Vec2D size,unsigned maxObjects = 64,QuadTreeCollisionBucket * parent=0);
	~QuadTreeCollisionBucket();

	bool AddObject(CollisionObject2DBase* object)override;
	CollisionObject2DBase* RemoveObject(CollisionObject2DBase* object)override;

	virtual bool UpdateObject(CollisionObject2DBase* object)override;

	virtual void PrintAllContents(unsigned depth=0)override;
	virtual void PrintAllCollisions()override;

	virtual void UpdateAllObjects()override;
	virtual void CheckAllCollision(struct CollisionFrame2D& frame)override;
	virtual bool ObjectIsWithinBucket(CollisionObject2DBase* object)override;
	virtual class Collision2D* CheckCollisionForObjectNoFrame(CollisionObject2DBase* object);
	virtual class Collision2D* CheckCollisionForObject(CollisionObject2DBase* object, CollisionFrame2D& frame)override;
	virtual class Collision2D* CheckObjectAgainstStaic(CollisionObject2DBase* object)override;
	virtual bool SweepCollision(CollisionObject2DBase* object, Vec2D newPosition, struct SweepCollision2D& response)override;
	virtual bool SweepCollisionHitTest(CollisionObject2DBase* object, Vec2D newPosition,struct SweepCollision2D& response)override;
};
