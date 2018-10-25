#pragma once

#include <mutex>
#include <Interfaces/ITickableObject.h>
#include <Math/Matrix44.h>
/*
*  Scene objects are objects that can be places in a level.
*  They do not have to be renderable and do not have any render logic in them.
*/
class VisibleComponentBase;
class PhysicsObjectBase;
class ComponentBase;

class ZoranEngine_EXPORT SceneObject : public ITickableObject
{
protected:
	ComponentBase * rootComponent;

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
	SceneObject(ComponentBase* component, std::string);
	virtual ~SceneObject();

	// Destroys this object removeing it from any part of the engine that it needs to
	virtual void Destroy();

	// default impl
	virtual void Tick(float deltaTime)override {}

	virtual void PreCaclModel() = 0;

    // Getter / Setter

	void SetRootComponent(ComponentBase* component);

	inline const Matrix44& GetModel() { return ModelMatrixCache; };
	virtual Matrix44 GetScaleMatrix4x4() = 0;

	inline const ComponentBase* GetRootComponent() { return rootComponent; }

	friend class ZoranEngine;
};
