#pragma once
#include <Physics\Collision\CollisionBucketBase.h>

class CollisionObject3DBase;
class SceneObject3D;
class PhysicsObject3DBase;

class CollisionBucket3DBase : public CollisionBucketBase
{
protected:
	class SceneObject3D * sceneObject;
	class CollisionObject3DBase* collision;

#pragma warning(push)
#pragma warning(disable:4251)
	// this should be split into dynamic / static objects for effeicinecy.
	// It's a lot of work though so unless we need it at some point, it will be left like this for now
	std::vector<class CollisionObject3DBase*> collisionObjects;
#pragma warning(pop)

protected:
	int FindObject(class CollisionObject3DBase* object);

public:
	CollisionBucket3DBase(std::string name, Vec3D pos, unsigned maxObjects = 32);
	virtual ~CollisionBucket3DBase();

	virtual bool AddObject(CollisionObject3DBase* object) = 0;
	virtual CollisionObject3DBase* RemoveObject(CollisionObject3DBase* object) = 0;

	
	virtual bool UpdateObject(CollisionObject3DBase* object) = 0;

	virtual bool ObjectIsWithinBucket(CollisionObject3DBase* object) = 0;
	virtual bool CheckObjectAgainstStaic(CollisionObject3DBase* object, struct CollisionResponse3D& response) = 0;
	virtual bool CheckCollisionForObject(CollisionObject3DBase* object, struct CollisionResponse3D& response) = 0;
	virtual bool SweepCollision(CollisionObject3DBase* object, Vec3D newPosition, struct SweepCollisionResponse3D& response) = 0;
	virtual bool SweepCollisionHitTest(CollisionObject3DBase* object, Vec3D newPosition, struct SweepCollisionResponse3D& response) = 0;
	virtual SceneObject* GetSceneObject()override;
};
