#include "stdafx.h"
#include "ComponentBase.h"
#include <assert.h>
#include <Utils/VectorAddons.hpp>

const Matrix44 ComponentBase::GetWorldTraverseUp(const Matrix44 & local)const
{
	Matrix44 world = this->localMatrix * local;
	if(parent)
		return parent->GetWorldTraverseUp(world);
	else return world;
}

ComponentBase::ComponentBase() : parent(0)
{
	subComponents = new std::vector<ComponentBase*>();
}

ComponentBase::~ComponentBase()
{
	delete subComponents;
}

void ComponentBase::AddSubComponent(ComponentBase * component)
{
	assert(component != 0 && "Can not Add NULL Sub Component");

	component->parent = this;
	subComponents->push_back(component);
}

void ComponentBase::RemoveSubComponent(ComponentBase * component)
{
	assert(component != 0 && "Can not Remove NULL Sub Component");

	remove(*subComponents,component);
}

const std::vector<ComponentBase*>& ComponentBase::GetSubComponenets() const
{
	return *subComponents;
}

ComponentBase * ComponentBase::GetSubComponenetAtIndex(unsigned index) const
{
	if(index < subComponents->size())
		return (*subComponents)[index];
	else return 0;
}

const Matrix44 ComponentBase::GetWorldMatrix() const
{
	if(parent)
		return parent->GetWorldTraverseUp(localMatrix);
	else return localMatrix;
}
