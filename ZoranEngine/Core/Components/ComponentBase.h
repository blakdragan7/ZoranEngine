#pragma once
#include <Core/PlatformTypes.h>
#include <Math/Matrix44.hpp>
#include <vector>

class ZoranEngine_EXPORT ComponentBase
{
protected:
	std::vector<ComponentBase*>* subComponents;
	ComponentBase* parent;

	Matrix44 localMatrix;

public:
	ComponentBase();
	virtual ~ComponentBase();

	void AddSubComponent(ComponentBase* component);
	void RemoveSubComponent(ComponentBase* component);

	const std::vector<ComponentBase*>& GetSubComponenets()const;
	ComponentBase* GetSubComponenetAtIndex(unsigned index)const;

	virtual const Matrix44& GetWorldMatrix()const = 0;
	inline const Matrix44& GetLocalMatrix()const { return localMatrix; };
};

