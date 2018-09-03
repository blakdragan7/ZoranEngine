#include "stdafx.h"

#include "ComponentBase.h"
#include "RigidBodyComponent.h"
#include "CollisionComponentBase.h"

#include <assert.h>
#include <Utils/VectorAddons.hpp>

#include <Rendering/RenderEngineBase.h>

#include <Physics/PhysicsEngine.h>

const Matrix44 ComponentBase::GetWorldTraverseUp(const Matrix44 & local)const
{
	Matrix44 world = this->localMatrix * local;
	if(parent)
		return parent->GetWorldTraverseUp(world);
	else return world;
}

ComponentBase::ComponentBase(ComponentType componentType) : componentType(componentType), parent(0), isDirty(false)
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

void ComponentBase::AddSubComponentsToRender(RenderEngineBase * engine)const
{
	if (GetIsVisibleComponent())
	{
		engine->AddComponent(((VisibleComponentBase*)this));
	}
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		(*subComponents)[i]->AddSubComponentsToRender(engine);
	}
}

void ComponentBase::RemoveSubComponentsFromRender(RenderEngineBase * engine)const
{
	if (GetIsVisibleComponent())
	{
		engine->RemoveComponent(((VisibleComponentBase*)this));
	}
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		(*subComponents)[i]->RemoveSubComponentsFromRender(engine);
	}
}

void ComponentBase::AddComponentsToPhysics(PhysicsEngine* engine)const
{
	if (GetIsPhysicsComponent())
	{
		RigidBodyComponent* rigid = (RigidBodyComponent*)this;
		engine->AddPhysicsObject(rigid->GetPhyicsObject());
	}
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		(*subComponents)[i]->AddComponentsToPhysics(engine);
	}
}

void ComponentBase::RemoveComponentsFromPhysics(PhysicsEngine* engine)const
{
	if (GetIsPhysicsComponent())
	{
		RigidBodyComponent* rigid = (RigidBodyComponent*)this;
		engine->RemoveObject(rigid->GetPhyicsObject());
	}
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		(*subComponents)[i]->RemoveComponentsFromPhysics(engine);
	}
}

void ComponentBase::AddComponentsToCollision(PhysicsEngine* engine)const
{
	if (GetIsCollisionComponent())
	{
		CollisionComponentBase* collision = (CollisionComponentBase*)this;
		engine->AddCollisionObject(collision->GetCollisionObject());
	}
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		(*subComponents)[i]->AddComponentsToCollision(engine);
	}
}

void ComponentBase::RemoveComponentsFromCollision(PhysicsEngine* engine)const
{
	if (GetIsCollisionComponent())
	{
		CollisionComponentBase* collision = (CollisionComponentBase*)this;
		engine->RemoveObject(collision->GetCollisionObject());
	}
	for (unsigned i = 0; i < subComponents->size(); i++)
	{
		(*subComponents)[i]->RemoveComponentsFromCollision(engine);
	}
}
