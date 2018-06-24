#pragma once
#include "Math/Vector3.h"
#include "Core/PlatformTypes.h"
#include <vector>

class PhysicsObjectBase;
class CollisionObjectBase;
class CollisionBucketBase;
struct CollisionResponse;
class SceneObject;
class ZoranEngine_EXPORT PhysicsEngine
{
private:
	CollisionBucketBase * collisionTree;
#pragma warning(push)
#pragma warning(disable:4251)
	std::vector<PhysicsObjectBase*> physicsObjects;
#pragma warning(pop)

public:
	PhysicsEngine();
	~PhysicsEngine();

	void CheckAllCollision();
	void SetupFor2D(Vector3D mapCenter, Vector3D mapSize);
	void SetupFor3D();

	CollisionBucketBase* GetCollisionBucketRoot();

	void UpdateAll(double deltaTime);

	void AddPhysicsObject(PhysicsObjectBase* object);
	void AddCollisionObject(CollisionObjectBase* object);
	void UpdateCollisionObject(CollisionObjectBase*);

	CollisionObjectBase* RemoveObject(CollisionObjectBase* object);
	PhysicsObjectBase* RemoveObject(PhysicsObjectBase* object);
};
