#pragma once
#include "PlatformTypes.h"
#include "Vector3.h"

class SceneObject;
class DRAGENGINE_EXPORT PhysicsObjectBase
{
private:
	unsigned physicsLayer;

	SceneObject* sceneObject;

public:
	PhysicsObjectBase(SceneObject* object);
	virtual ~PhysicsObjectBase();

	void SetSceneObject(SceneObject* object);

	virtual unsigned GetPhysicsLayer();
	void SetPhysicsLayer(unsigned layer);

	virtual bool CollidesWith(Vector3D pos) = 0;
	virtual bool CollidesWith(PhysicsObjectBase* other) = 0;
	virtual Vector3D GetClosestPointTo(Vector3D pos) = 0;

	Vector3D GetScenePos();
	SceneObject* GetSceneObject();
};
