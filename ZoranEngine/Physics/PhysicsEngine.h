#pragma once
#include "Math/Vector3.h"
#include "Core/PlatformTypes.h"
#include <vector>

class PhysicsObjectBase;
class CollisionObjectBase;
class CollisionBucket2DBase;
class CollisionBucket3DBase;
struct CollisionResponse3D;
class SceneObject;
class ZoranEngine_EXPORT PhysicsEngine
{
private:
	CollisionBucket2DBase * collisionTree2D;
	CollisionBucket3DBase * collisionTree3D;

#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<PhysicsObjectBase*> physicsObjects;
#pragma warning(pop)

	bool is3D;

public:
	PhysicsEngine();
	~PhysicsEngine();

	void CheckAllCollision();
	void SetupFor2D(Vector2D mapCenter, Vector2D mapSize);
	void SetupFor3D();

	class CollisionBucket2DBase* GetCollisionBucketRootFor2D();
	class CollisionBucket3DBase* GetCollisionBucketRootFor3D();
	class CollisionBucketBase* GetCollisionBucketRoot();

	void UpdateAll(double deltaTime);

	void AddPhysicsObject(PhysicsObjectBase* object);
	void AddCollisionObject(CollisionObjectBase* object);
	void UpdateCollisionObject(CollisionObjectBase*);

	CollisionObjectBase* RemoveObject(CollisionObjectBase* object);
	PhysicsObjectBase* RemoveObject(PhysicsObjectBase* object);
};
