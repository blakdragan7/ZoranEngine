#include "stdafx.h"
#include "PhysicsObjectBase.h"
#include "Core/SceneObject.h"
#include "Core/ZoranEngine.h"
#include "PhysicsEngine.h"
#include "Physics/Collision/CollisionBucketBase.h"
#include "Physics/Collision/CollisionObjectBase.h"
#include "Physics/Collision/3D/BoxCollisionObject.h"
#include "Physics/Collision/3D/SphereCollisionObject.h"
#include "Math/MathHelpers.h"

#include <cmath>
const double EulerConstant = std::exp(1.0);

void PhysicsObjectBase::RegisterPhysicsObject()
{
	pEngine->AddPhysicsObject(this);
}

bool PhysicsObjectBase::SweepCollisionTo(Vec3D position, SweepCollisionResponse & response)
{
	return pEngine->GetCollisionBucketRoot()->SweepCollision(sceneObject->GetCollision(),position, response);
}

bool PhysicsObjectBase::FastSweepCollision(Vec3D position)
{
	Vec3D startingPosition = sceneObject->GetPosition();
	Vec3D size = sceneObject->GetCollision()->GetSize() / 2.0;

	double minPositionx = min(startingPosition.x, position.x);
	double maxPositionx = max(startingPosition.x, position.x);

	double minPositiony = min(startingPosition.y, position.y);
	double maxPositiony = max(startingPosition.y, position.y);

	double minPositionz = min(startingPosition.z, position.z);
	double maxPositionz = max(startingPosition.z, position.z);

	sweepCollisionBox->minPos.x = minPositionx - size.x;
	sweepCollisionBox->maxPos.x = maxPositionx + size.x;

	sweepCollisionBox->minPos.y = minPositiony - size.y;
	sweepCollisionBox->maxPos.y = maxPositiony + size.y;

	sweepCollisionBox->minPos.z = minPositionz - size.z;
	sweepCollisionBox->maxPos.z = maxPositionz + size.z;

	CollisionResponse unused;
	return pEngine->GetCollisionBucketRoot()->CheckObjectAgainstStaic(sweepCollisionBox, unused);
}

PhysicsObjectBase::PhysicsObjectBase(SceneObject * object)
{
	sceneObject = object;
	shouldSimulate = false;
	gravity = Vec3D(0, -980, 0);
	gravityNormal = Vec3D(0, -1,0);
	drag = 0.99;
	otherFriction = 1.0;
	friction = 0.99;
	mass = 200;
	restitution = 1.0;
	isOnGround = false;
	useSweptCollision = false;

	sweepCollisionBox = new BoxCollisionObject(Vec3D(),Vec3D(),object,CD_Static,BOX_COLLISION);
}

PhysicsObjectBase::~PhysicsObjectBase()
{
	// Not in charge of deleting Scene or collision objects, thats the engines job
	// We do need to unregister though

	delete sweepCollisionBox;

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

void PhysicsObjectBase::SetSweptCollision(bool newSwept)
{
	useSweptCollision = newSwept;
}

void PhysicsObjectBase::SetVeloctiy(Vec3D Velocity)
{
	this->velocity = Velocity;
}

void PhysicsObjectBase::SetGravity(Vector3D gravity)
{
	this->gravity = gravity;
	gravityNormal = gravity;
	gravityNormal.normalize();
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

			sceneObject->Translate(response.penetration);
			
			Vec3D Vel = velocity - other->velocity;
			Vector3D F = (-response.normal * ((1.0 + restitution) * Vel.dot(response.normal))) / (mass + other->mass);
			velocity += F * other->mass * 0.99;
			// very complicated and realistic isOnGround code. need to probably optmize this somehow later
			if (isOnGround == false)
			{
				Vec3D cross = response.normal.cross(gravity);
				double dot = response.normal.dot(gravity);
				if (cross.getMagnitudeSqr() == 0 && dot > 0)
				{
					Vec3D velocityGravNorm = (velocity + 10 * gravityNormal) * gravityNormal.getAbs();
					if (gravity.dot(velocityGravNorm) >= 0.0)
					{
						isOnGround = true;
						velocity += velocityGravNorm;
						otherFriction = other->friction;
					}
				}
			}
			std::cout << "collision\n";
		}
	}
}

bool PhysicsObjectBase::SweepToo(Vec3D targetPosition, SweepCollisionResponse & response)
{
	if (FastSweepCollision(targetPosition))
	{
		return SweepCollisionTo(targetPosition, response);
	}

	return false;
}

void PhysicsObjectBase::ApplyForce(Vec3D Force)
{
	this->velocity += Force;
	isOnGround = false;
	otherFriction = 1.0;
}

void PhysicsObjectBase::Update(double deltaTime)
{
	if (shouldSimulate)
	{
		if(isOnGround == false)velocity += gravity*deltaTime;
		else 
			velocity *= (friction*otherFriction);
		velocity *= drag;

		if (useSweptCollision)
		{
			Vec3D target = sceneObject->GetPosition() + (velocity*deltaTime);
			SweepCollisionResponse response;
			if (SweepToo(target,response))
			{
				Vec3D actual = GetScenePos() + (velocity * deltaTime * response.timeHit);
				sceneObject->SetPosition(actual);
				if (response.timeHit < 1.0)
				{
					double invTime = (1.0 - response.timeHit);
					velocity *= invTime;
				}
				OnCollision(response.collisionResponse);

			}
			else
				sceneObject->SetPosition(target);
		}
		else
		{
			sceneObject->Translate(velocity*deltaTime);
		}
	}
}

Vec3D PhysicsObjectBase::GetVelocity()
{
	return velocity;
}

Vec3D PhysicsObjectBase::GetScenePos() { return sceneObject->GetPosition(); }
SceneObject* PhysicsObjectBase::GetSceneObject() { return sceneObject; }