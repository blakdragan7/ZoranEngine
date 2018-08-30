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
};

