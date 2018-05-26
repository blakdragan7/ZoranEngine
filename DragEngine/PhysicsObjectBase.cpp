#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "SceneObject.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"

void PhysicsObjectBase::RegisterPhysicsObject()
{
	pEngine->AddPhysicsObject(this);
}

PhysicsObjectBase::PhysicsObjectBase(SceneObject * object)
{
	sceneObject = object;
	collision = sceneObject->GetCollision();
	shouldSimulate = false;
	gravity = -9.80;
	RegisterPhysicsObject();
}

PhysicsObjectBase::~PhysicsObjectBase()
{
	// Not in charge of deleting Scene or collision objects, thats the engines job
	// We do need to unregister though

	pEngine->RemoveObject(this);
}

void PhysicsObjectBase::StartPhysicsSim()
{
	shouldSimulate = true;
}

void PhysicsObjectBase::StopPhysicsSim()
{
	shouldSimulate = false;
}

void PhysicsObjectBase::SetVeloctiy(Vec3D Velocity)
{
	this->velocity = Velocity;
}

void PhysicsObjectBase::SetGravity(double gravity)
{
	this->gravity = gravity;
}

void PhysicsObjectBase::ApplyForce(Vec3D Force)
{
	this->velocity += Force;
}

void PhysicsObjectBase::Update(double deltaTime)
{
	if (shouldSimulate)
	{
		velocity += gravity;
		velocity *= drag;

		sceneObject->SetPosition(sceneObject->GetPosition() + (velocity * deltaTime));
	}
}

Vec3D PhysicsObjectBase::GetVelocity()
{
	return velocity;
}

Vec3D PhysicsObjectBase::GetScenePos() { return sceneObject->GetPosition(); }
SceneObject* PhysicsObjectBase::GetSceneObject() { return sceneObject; }