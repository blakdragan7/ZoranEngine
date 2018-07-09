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
const double EulerConstant = std::exp(1.0);

void PhysicsObjectBase::RegisterPhysicsObject()
{
	pEngine->AddPhysicsObject(this);
}

PhysicsObjectBase::PhysicsObjectBase(SceneObject * object)
{
	currentDeltaTime = 0;
	sceneObject = object;
	shouldSimulate = false;
	drag = 0.99;
	otherFriction = 1.0;
	friction = 0.99;
	mass = 200;
	restitution = 1.0;
	isOnGround = false;
	useSweptCollision = false;
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

void PhysicsObjectBase::SetFriction(double friction)
{
	this->friction = friction;
}

void PhysicsObjectBase::SetRestitution(double restitution)
{
	this->restitution = restitution;
}

void PhysicsObjectBase::SetSweptCollision(bool newSwept)
{
	useSweptCollision = newSwept;
}

SceneObject* PhysicsObjectBase::GetSceneObject() { return sceneObject; }