#pragma once
#include "Core/PlatformTypes.h"
#include "Math/Vector3.h"

#define NO_COLLISION 0

enum CollisionDynamics
{
	CD_Static=0,
	CD_Dynamic=1
};

#define COLLISION_LAYER_GROUND 0
#define COLLISION_LAYER_DYNAMIC 1

class ZoranEngine_EXPORT CollisionObjectBase
{
private:
	unsigned collisionType;
	unsigned collisionLayer;
	SceneObject* sceneObject;
	class PhysicsObjectBase* physicsObject;
	CollisionDynamics collisionDynamics;
	
public:
	bool isDirty;

public:
	CollisionObjectBase(SceneObject* object, CollisionDynamics collisionDynamics = CD_Dynamic, unsigned collisionType = NO_COLLISION);
	virtual ~CollisionObjectBase();

	void SetSceneObject(SceneObject* object);
	void SetPhysicsObject(PhysicsObjectBase* object);
	virtual void SetBoundsBySceneObject() = 0;

	inline unsigned GetCollisionType() { return collisionType; }
	virtual unsigned GetCollisionLayer();
	void SetCollisionLayer(unsigned layer);

	void SetDynamics(CollisionDynamics dynamics);

	CollisionDynamics GetDynamics();
	SceneObject* GetSceneObject();
	PhysicsObjectBase* GetPhysicsObject();
};
