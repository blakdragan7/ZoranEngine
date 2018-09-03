#pragma once
#include <Core/PlatformTypes.h>
#include <Math/Matrix44.hpp>
#include <vector>

enum ComponentType
{
	CT_Visible,
	CT_Physics,
	CT_Collision,
	CT_Audio,
	CT_None
};

class RenderEngienBase;
class PhysicsEngine;
class CollisionBucketBase;

class ZoranEngine_EXPORT ComponentBase
{
protected:
	std::vector<ComponentBase*>* subComponents;
	ComponentBase* parent;

	Matrix44 localMatrix;

	bool isDirty;
	ComponentType componentType;

private:
	const Matrix44 GetWorldTraverseUp(const Matrix44& local)const;

public:
	ComponentBase(ComponentType componentType);
	virtual ~ComponentBase();

	void AddSubComponent(ComponentBase* component);
	void RemoveSubComponent(ComponentBase* component);

	const std::vector<ComponentBase*>& GetSubComponenets()const;
	ComponentBase* GetSubComponenetAtIndex(unsigned index)const;

	inline bool GetIsVisibleComponent()const { return componentType == CT_Visible; }
	inline bool GetIsCollisionComponent()const { return componentType == CT_Collision; }
	inline bool GetIsPhysicsComponent()const { return componentType == CT_Physics; }
	inline bool GetIsAudioComponent()const { return componentType == CT_Audio; }

	/* must be calculate every frame ... may need a better way to do this */
	const Matrix44 GetWorldMatrix()const;
	/* returns local transform matrix */
	inline const Matrix44& GetLocalMatrix()const { return localMatrix; };
	/* Used to re create local matrix when it's marked dirty, 
	   this is virtual because there is a difference in how it's created for 2d vs 3d */
	virtual void CalculateLocalMatrix() = 0;
	/* Helper function for adding and removing all sub components and this component to the engine.  */
	void AddSubComponentsToRender(RenderEngineBase* engine)const;
	void RemoveSubComponentsFromRender(RenderEngineBase * engine)const;
	void AddComponentsToPhysics(PhysicsEngine* engine)const;
	void RemoveComponentsFromPhysics(PhysicsEngine* engine)const;
	void AddComponentsToCollision(PhysicsEngine* engine)const;
	void RemoveComponentsFromCollision(PhysicsEngine* engine)const;
};

