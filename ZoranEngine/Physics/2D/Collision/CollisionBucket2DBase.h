#pragma once
#include <Physics\Collision\CollisionBucketBase.h>
#include <Math/Vector2.h>

class ZoranEngine_EXPORT CollisionBucket2DBase : public CollisionBucketBase
{
protected:
	class SceneObject2D * sceneObject;
	class CollisionObject2DBase* collision;

#pragma warning(push)
#pragma warning(disable:4251)
	// this should be split into dynamic / static objects for effeicinecy.
	// It's a lot of work though so unless we need it at some point, it will be left like this for now
	std::vector<class CollisionObject2DBase*> collisionObjects;
#pragma warning(pop)

protected:
	int FindObject(class CollisionObject2DBase* object);

public:
	CollisionBucket2DBase(std::string name, Vec2D pos, unsigned maxObjects = 32);
	virtual ~CollisionBucket2DBase();

	virtual bool AddObject(CollisionObject2DBase* object) = 0;
	virtual CollisionObject2DBase* RemoveObject(CollisionObject2DBase* object) = 0;

	virtual bool UpdateObject(CollisionObject2DBase* object) = 0;

	virtual bool ObjectIsWithinBucket(CollisionObject2DBase* object) = 0;
	virtual class Collision2D* CheckObjectAgainstStaic(CollisionObject2DBase* object) = 0;
	virtual void CheckCollisionForObject(CollisionObject2DBase* object, CollisionFrame2D& frame) = 0;
	virtual bool SweepCollision(CollisionObject2DBase* object, Vec2D newPosition, struct SweepCollision2D& response) = 0;
	virtual bool SweepCollisionHitTest(CollisionObject2DBase* object, Vec2D newPosition, struct SweepCollision2D& response) = 0;
	virtual SceneObject* GetSceneObject()override;
};

