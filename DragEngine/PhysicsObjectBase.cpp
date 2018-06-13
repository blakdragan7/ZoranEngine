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
	mass = 200;
	restitution = 1.0;
	isOnGround = false;
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

void PhysicsObjectBase::SetMass(double mass)
{
	this->mass = mass;
}

void PhysicsObjectBase::SetRestitution(double restitution)
{
	this->restitution = restitution;
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
	if (shouldSimulate)
	{
		if (response.collided)
		{
			CollisionObjectBase* collision = response.objectBounds[0];
			CollisionObjectBase* collision2 = response.objectBounds[1];

			PhysicsObjectBase* other = response.collidedObjects[1];

			Vec3D penetration;

			if (collision->GetCollisionType() == BOX_COLLISION)
			{
				BoxCollisionObject* bcollision = (BoxCollisionObject*)collision;
				Vec3D size = bcollision->GetSize();
				penetration = response.normal * (size - response.intersection.getAbs());
				sceneObject->Translate(penetration);
			}
			else if (collision->GetCollisionType() == SPHERE_COLLISION)
			{
				SphereCollisionObject* scollision = (SphereCollisionObject*)collision;
				penetration = response.normal * (scollision->GetRadiusSqr() - response.intersection.getMagnitudeSqr());
				sceneObject->Translate(penetration);
			}
			Vec3D Vel = velocity - other->velocity;
			Vector3D F = (-response.normal * ((1.0 + restitution) * Vel.dot(response.normal))) / (mass + other->mass);
			velocity += F * mass;
			if (velocity.nearlyEquals(0))
			{
				isOnGround = true;
			}
		}
	}
}

void PhysicsObjectBase::ApplyForce(Vec3D Force)
{
	this->velocity += Force;
	isOnGround = false;
}

void PhysicsObjectBase::Update(double deltaTime)
{
	if (shouldSimulate)
	{
		if(isOnGround == false)velocity += gravity;
		velocity *= drag;

#ifdef CON_COLLISION
		sceneObject->TrySetPosition(sceneObject->GetPosition() + (velocity * deltaTime));
#else
		sceneObject->Translate(velocity * deltaTime);
#endif
	}
}

Vec3D PhysicsObjectBase::GetVelocity()
{
	return velocity;
}

Vec3D PhysicsObjectBase::GetScenePos() { return sceneObject->GetPosition(); }
SceneObject* PhysicsObjectBase::GetSceneObject() { return sceneObject; }