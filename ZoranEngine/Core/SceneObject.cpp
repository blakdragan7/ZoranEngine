#include <stdafx.h>
#include "SceneObject.h"

#include <Core/ThreadBase.h>
#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/PhysicsObjectBase.h>
#include <Rendering/RenderEngineBase.h>
#include <Rendering/ShaderProgramBase.h>
#include <Rendering/RenderedObjectBase.h>
#include <Physics/Collision/CollisionBucketBase.h>
#include <Physics/Collision/CollisionObjectBase.h>
#include <Physics/3D/Collision/AABBoxCollisionObject.h>
#include <Physics/3D/Collision/SphereCollisionObject.h>

static unsigned long long NextID = 0;

void SceneObject::WaitForMutex()
{
	while (mutex.try_lock() == false)ThreadBase::Sleep(0.01F);
}

void SceneObject::UnlockMutex()
{
	mutex.unlock();
}

void SceneObject::SetShaderProgram(ShaderProgramBase * newShaderProgram)
{
	shaderProgram = newShaderProgram;
}

void SceneObject::SetRenderedObject(RenderedObjectBase * newRenderedObject)
{
	renderedObject = newRenderedObject;
}

SceneObject::SceneObject(std::string name)
{
	willEverTick = false;

	renderEngine = zEngine->GetRenderer();
	renderedObject = renderEngine->CreateRenderedObject();

	hasCollision = false;

	this->readableName = name;
	this->ID = NextID++;
}

SceneObject::SceneObject(std::string name, RenderEngineBase* engine)
{
	willEverTick = false;

	this->readableName = name;
	this->ID = NextID++;

	hasCollision = false; 
	renderEngine = engine;

	renderedObject = 0;
}

SceneObject::~SceneObject()
{
	if(renderedObject)delete renderedObject;
}


void SceneObject::PostRender()
{
	// profit ?
}

void SceneObject::RenderScene()
{
	renderedObject->RenderObject();
}

void SceneObject::PreRender()
{
}

void SceneObject::Destroy()
{
	zEngine->RemoveTickableObject(this);
	zEngine->DestroySceneObject(this);
}
