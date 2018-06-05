#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "SceneObject.h"
#include "DragEngine.h"
#include "PhysicsEngine.h"
#include "CollisionBucketBase.h"
#include "CollisionObjectBase.h"
#include "BoxCollisionObject.h"
#include "SphereCollisionObject.h"

void PhysicsObjectBase::RegisterPhysicsObject()
{
	pEngine->AddPhysicsObject(this);
}

PhysicsObjectBase::PhysicsObjectBase(SceneObject * object)
{
	sceneObject = object;
	shouldSimulate = false;
	gravity = Vec3D(0, -.00980,0);
	drag = 0.99;
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

void PhysicsObjectBase::OnCollision(CollisionResponse &response)
{
	if (response.collided)
	{
		CollisionObjectBase* collision = response.objectBounds[0];
		CollisionObjectBase* collision2 = response.objectBounds[1];

		if (collision->GetCollisionType() == BOX_COLLISION)
		{
			BoxCollisionObject* bcollision = (BoxCollisionObject*)collision;
			Vec3D size = bcollision->GetSize();
			Vec3D penetration = response.normal * (size - response.intersection.getAbs());
			sceneObject->Translate(penetration);
		}
		else if (collision->GetCollisionType() == SPHERE_COLLISION)
		{
			SphereCollisionObject* scollision = (SphereCollisionObject*)collision;
			Vec3D penetration = response.normal * (scollision->GetRadiusSqr() - response.intersection.getMagnitudeSqr());
			sceneObject->Translate(penetration);
		}
	
		Vector3D r = velocity - (2.0 * (velocity.dot(response.normal)) * response.normal);
		velocity = r;
	}
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

		sceneObject->TrySetPosition(sceneObject->GetPosition() + (velocity * deltaTime));
	}
}

Vec3D PhysicsObjectBase::GetVelocity()
{
	return velocity;
}

Vec3D PhysicsObjectBase::GetScenePos() { return sceneObject->GetPosition(); }
SceneObject* PhysicsObjectBase::GetSceneObject() { return sceneObject; }