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
#include <Physics/Collision/3D/BoxCollisionObject.h>
#include <Physics/Collision/3D/SphereCollisionObject.h>

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

	collision = 0;  
	hasCollision = false;

	this->readableName = name;
	this->ID = NextID++;
}

SceneObject::SceneObject(std::string name, RenderEngineBase* engine)
{
	willEverTick = false;

	this->readableName = name;
	this->ID = NextID++;

	collision = 0;  
	hasCollision = false; 
	renderEngine = engine;
}

SceneObject::~SceneObject()
{
	if (collision)delete collision;
	if (physicsObject)delete physicsObject;
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
	if (shaderProgram)
	{
		shaderProgram->BindProgram();
		shaderProgram->SetupShaderFromSceneObject(this);
	}
}

void SceneObject::Destroy()
{
	if(collision)pEngine->RemoveObject(collision);
	if(physicsObject)pEngine->RemoveObject(physicsObject);
	zEngine->RemoveTickableObject(this);
	zEngine->DestroySceneObject(this);
}
