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
	while (mutex->try_lock() == false)ThreadBase::Sleep(0.01F);
}

void SceneObject::UnlockMutex()
{
	mutex->unlock();
}

void SceneObject::SetShaderProgram(ShaderProgramBase * newShaderProgram)
{
	shaderProgram = newShaderProgram;
}

SceneObject::SceneObject(std::string name) : rootComponent(0) , willEverTick(false), hasCollision(false)
{
	this->ID = NextID++;

	this->mutex = new std::mutex();
	this->readableName = new std::string(name);
}

SceneObject::~SceneObject()
{
	delete readableName;
	delete mutex;
}

void SceneObject::Destroy()
{
	zEngine->RemoveTickableObject(this);
	zEngine->DestroySceneObject(this);
}
