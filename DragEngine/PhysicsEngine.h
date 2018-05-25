#pragma once
#include "Vector3.h"
#include "PlatformTypes.h"
class PhysicsObjectBase;
class CollisionObjectBase;
class CollisionBucketBase;
class DRAGENGINE_EXPORT PhysicsEngine
{
private:
	CollisionBucketBase * collisionTree;

public:
	PhysicsEngine();
	~PhysicsEngine();

	void CheckAllCollision();
	void SetupFor2D(Vector3D mapCenter, Vector3D mapSize);
	void SetupFor3D();

	void UpdateAll(double deltaTime);

	void AddCollisionObject(CollisionObjectBase* object);
	CollisionObjectBase* RemoveObject(CollisionObjectBase* object);
};
