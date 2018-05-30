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
	gravity = Vec3D(0, -.00980,0);
	drag = 0.999;
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

void PhysicsObjectBase::SetGravity(Vector3D gravity)
{
	this->gravity = gravity;
}

void PhysicsObjectBase::CollidedWith(Vector3D point, Vector3D normal, PhysicsObjectBase * other)
{
	Vector3D r = velocity - (2.0 * (velocity.dot(normal)) * normal);
	velocity = r;
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