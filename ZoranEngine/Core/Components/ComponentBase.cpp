#include "stdafx.h"
#include "ComponentBase.h"
#include <assert.h>
#include <Utils/VectorAddons.hpp>

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
