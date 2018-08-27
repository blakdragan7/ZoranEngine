#pragma once

#include <mutex>
#include "Core/TickableObject.h"
#include <Math/Matrix44.hpp>
/*
*  Scene objects are objects that can be places in a level.
*  They do not have to be renderable and do not have any render logic in them.
*/

class CollisionObject2DBase;
class PhysicsObjectBase;
class ComponentBase;

class ZoranEngine_EXPORT SceneObject : public TickableObject
{
protected:
	ComponentBase*		rootComponent;

	unsigned long long ID;
	std::mutex* mutex;
	bool hasCollision;

	bool willEverTick;

public:

	std::string* readableName;

protected:
	void WaitForMutex();
	void UnlockMutex();

	Matrix44 ModelMatrixCache;

public:
	SceneObject(std::string);
	virtual ~SceneObject();

	// Destroys this object removeing it from any part of the engine that it needs to
	virtual void Destroy();

	// default impl
	virtual void Tick(float deltaTime)override {}

	virtual void PreCaclModel() = 0;

    // Getter / Setter

	inline const Matrix44& GetModel() { return ModelMatrixCache; };
	virtual Matrix44 GetScaleMatrix4x4() = 0;

	virtual class CollisionObjectBase* GetCollision()const = 0;
	virtual class PhysicsObjectBase* GetPhysics()const = 0;
};

