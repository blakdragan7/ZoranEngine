#pragma once
#include "Math/Vector3.h"
#include <vector>
#include <mutex>



class ZoranEngine_EXPORT CollisionBucketBase
{
protected:
	unsigned maxObjects;

#pragma warning(push)
#pragma warning(disable:4251)
	std::mutex mutex;
#pragma warning(pop)

public:
	CollisionBucketBase(unsigned maxObjects = 32);
	virtual ~CollisionBucketBase();

	// Debug Prints
	virtual void PrintAllCollisions() = 0;
	virtual void PrintAllContents(unsigned depth = 0) = 0;
	virtual SceneObject* GetSceneObject() = 0;
	virtual void CheckAllCollision(struct CollisionFrame2D& frame) = 0;
	virtual void UpdateAllObjects() = 0;
};  

