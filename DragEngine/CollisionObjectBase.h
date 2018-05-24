#pragma once
#include "PlatformTypes.h"
#include "Vector3.h"

#define NO_COLLISION 0

class SceneObject;
class DRAGENGINE_EXPORT CollisionObjectBase
{
private:
	unsigned collisionType;
	unsigned collisionLayer;
	SceneObject* sceneObject;

public:
	CollisionObjectBase(SceneObject* object, unsigned collisionType = NO_COLLISION);
	virtual ~CollisionObjectBase();

	void SetSceneObject(SceneObject* object);

	inline unsigned GetCollisionType() { return collisionType; }
	virtual unsigned GetCollisionLayer();
	void SetCollisionLayer(unsigned layer);

	virtual bool CollidesWith(Vector3D pos) = 0;
	virtual bool CollidesWith(CollisionObjectBase* other) = 0;
	virtual Vector3D GetClosestPointTo(Vector3D pos) = 0;

	Vector3D GetScenePos();
	SceneObject* GetSceneObject();
};