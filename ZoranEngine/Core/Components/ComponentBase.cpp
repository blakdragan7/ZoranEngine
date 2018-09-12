#include "stdafx.h"

#include "ComponentBase.h"
#include "RigidBodyComponent.h"
#include "CollisionComponentBase.h"

#include <assert.h>
#include <Utils/VectorAddons.hpp>

#include <Core/SceneObject.h>

#include <Rendering/RenderEngineBase.h>

#include <Physics/PhysicsEngine.h>

const Matrix44 ComponentBase::GetWorldTraverseUp(const Matrix44 & local)const
{
	Matrix44 world = this->localMatrix * local;
	if(parent)
		return parent->GetWorldTraverseUp(world);
	else return world;
}

ComponentBase::ComponentBase() : parent(0), isDirty(false)
{
	subComponents = new std::vector<ComponentBase*>();
}

ComponentBase::~ComponentBase()
{
	for (ComponentBase* component : *subComponents)
	{
		delete component;
	}

	delete subComponents;
}

void ComponentBase::AddSubComponent(ComponentBase * component)
{
	assert(component != 0 && "Can not Add NULL Sub Component");

	component->parent = this;
	component->SetOwner(owner);
	subComponents->push_back(component);
}

void ComponentBase::RemoveSubComponent(ComponentBase * component)
{
	assert(component != 0 && "Can not Remove NULL Sub Component");

	remove(*subComponents,component);
	component->SetOwner(0);
}

const std::vector<ComponentBase*>& ComponentBase::GetSubComponenets() const
{
	return *subComponents;
}

ComponentBase * ComponentBase::GetSubComponenetAtIndex(unsigned index) const
{
	if(index < subComponents->size())
		return (*subComponents)[index];
	else
	{
		Log(LogLevel_Error,"Trying to Get sub component at  %i when size is %i",index,subComponents->size());
		return 0;
	}
}

const Matrix44 ComponentBase::GetWorldMatrix() const
{
	if(parent)
		return parent->GetWorldTraverseUp(localMatrix);
	else return localMatrix;
}

void ComponentBase::TransferSubComponentsTo(ComponentBase * component)
{
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		ComponentBase* comp = (*subComponents)[i];
		component->AddSubComponent(comp);
	}

	subComponents->clear();
}

const char* ComponentBase::GetSceneName()const
{
	if (owner != NULL)
	{
		return owner->readableName->c_str();
	}

	return 0;
}

std::string *ComponentBase::GetSceneNameStr()const
{
	if (owner != NULL)
	{
		return owner->readableName;
	}

	return 0;
}
