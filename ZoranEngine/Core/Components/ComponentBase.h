#pragma once
#include <Core/PlatformTypes.h>
#include <Math/Matrix44.hpp>
#include <vector>

class RenderEngineBase;
class PhysicsEngine;
class CollisionBucketBase;
class SceneObject;
class ZoranEngine_EXPORT ComponentBase
{
private:
	SceneObject * owner;

protected:
	std::vector<ComponentBase*>* subComponents;
	ComponentBase* parent;

	Matrix44 localMatrix;

	bool isDirty;

private:
	const Matrix44 GetWorldTraverseUp(const Matrix44& local)const;

public:
	ComponentBase();
	virtual ~ComponentBase();

	void AddSubComponent(ComponentBase* component);
	void RemoveSubComponent(ComponentBase* component);

	const std::vector<ComponentBase*>& GetSubComponenets()const;
	ComponentBase* GetSubComponenetAtIndex(unsigned index)const;

	/* must be calculate every frame ... may need a better way to do this */
	const Matrix44 GetWorldMatrix()const;
	/* returns local transform matrix */
	inline const Matrix44& GetLocalMatrix()const { return localMatrix; };
	/* Used to re create local matrix when it's marked dirty, 
	   this is virtual because there is a difference in how it's created for 2d vs 3d */
	virtual void CalculateLocalMatrix() = 0;

	const char* GetSceneName();
	std::string *GetSceneNameStr();

	inline void SetOwner(SceneObject* owner) { this->owner = owner; }
	inline SceneObject* GetOwner() { return owner; }
};

