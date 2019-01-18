#include <stdafx.h>
#include "SceneObject.h"

#include <Core/ThreadBase.h>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/ShaderProgramBase.h>
#include <Rendering/RenderedObjectBase.h>
#include <Core/Components/ComponentBase.h>

static unsigned long long NextID = 0;

void SceneObject::WaitForMutex()
{
	while (mutex->try_lock() == false)ThreadBase::Sleep(0.01F);
}

void SceneObject::UnlockMutex()
{
	mutex->unlock();
}

SceneObject::SceneObject(std::string name) : rootComponent(0) , willEverTick(false), hasCollision(false)
{
	this->ID = NextID++;

	this->mutex = new std::mutex();
	this->readableName = new std::string(name);
}

SceneObject::SceneObject(ComponentBase* rootComponent,std::string name) : rootComponent(rootComponent), willEverTick(false), hasCollision(false)
{
	this->ID = NextID++;

	this->mutex = new std::mutex();
	this->readableName = new std::string(name);
}

SceneObject::~SceneObject()
{
	if(rootComponent)delete rootComponent;
	delete readableName;
	delete mutex;
}

void SceneObject::Destroy()
{
	zEngine->DestroySceneObject(this);
}

void SceneObject::SetRootComponent(ComponentBase * component)
{
	component->SetOwner(this);
	if (rootComponent)
	{
		rootComponent->TransferSubComponentsTo(component);
		delete rootComponent;
	}
	rootComponent = component;
}
