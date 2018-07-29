#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "Core/SceneObject.h"
#include "Core/ZoranEngine.h"
#include "PhysicsEngine.h"
#include "Physics/Collision/CollisionBucketBase.h"
#include "Physics/Collision/CollisionObjectBase.h"
#include "Physics/3D/Collision/AABBoxCollisionObject.h"
#include "Physics/3D/Collision/SphereCollisionObject.h"
#include "Math/MathHelpers.h"

#include <cmath>
const float EulerConstant = std::exp(1.0f);

void PhysicsObjectBase::RegisterPhysicsObject()
{
	pEngine->AddPhysicsObject(this);
}

PhysicsObjectBase::PhysicsObjectBase(SceneObject * object)
{
	currentDeltaTime = 0;
	sceneObject = object;
	shouldSimulate = false;
	drag = 0.99f;
	otherFriction = 1.0f;
	friction = 0.1f;
	restitution = 0.9f;
	isOnGround = false;
	useSweptCollision = false;
	calculatedFriction = sqrt(0.4);
	SetMass(200);
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

void PhysicsObjectBase::SetMass(float mass)
{
	if (mass >= FLT_MAX || mass == 0)
	{
		this->mass = mass;
		invMass = 0;
	}
	else
	{
		this->mass = mass;
		invMass = (mass != 0 ? 1.0f / mass : 0.0f);
	}
}

void PhysicsObjectBase::SetFriction(float friction)
{
	this->friction = friction;
}

void PhysicsObjectBase::SetRestitution(float restitution)
{
	this->restitution = restitution;
}

void PhysicsObjectBase::SetSweptCollision(bool newSwept)
{
	useSweptCollision = newSwept;
}

SceneObject* PhysicsObjectBase::GetSceneObject() { return sceneObject; }