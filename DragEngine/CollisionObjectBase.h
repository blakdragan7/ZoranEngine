#pragma once
#include "PlatformTypes.h"
#include "Vector3.h"

#define NO_COLLISION 0

enum CollisionDynamics
{
	CD_Static=0,
	CD_Dynamic=1
};

class SceneObject;
class PhysicsObjectBase;
class DRAGENGINE_EXPORT CollisionObjectBase
{
private:
	unsigned collisionType;
	unsigned collisionLayer;
	SceneObject* sceneObject;
	PhysicsObjectBase* physicsObject;
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

	virtual bool CollidesWith(Vector3D pos) = 0;
	virtual bool CollidesWith(CollisionObjectBase* other) = 0;
	virtual Vector3D GetClosestPointTo(Vector3D pos) = 0;
	virtual Vector3D GetNormalBetween(CollisionObjectBase* other) = 0;
	virtual Vector3D GetSize() = 0;

	virtual Vector3D GetPenetration(CollisionObjectBase* other, Vec3D normal);

	void SetDynamics(CollisionDynamics dynamics);

	CollisionDynamics GetDynamics();
	Vector3D GetScenePos();
	SceneObject* GetSceneObject();
	PhysicsObjectBase* GetPhysicsObject();
};
