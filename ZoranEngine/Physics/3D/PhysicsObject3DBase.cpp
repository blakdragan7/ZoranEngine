#include "stdafx.h"
#include "PhysicsObject3DBase.h"

#include <Core/ZoranEngine.h>
#include <Math/MathHelpers.h>
#include <Physics/PhysicsEngine.h>
#include <Core/3D/Components/Component3DBase.h>
#include <Physics/3D/Collision/AABBoxCollisionObject.h>
#include <Physics/3D/Collision/CollisionBucket3DBase.h>
#include <Physics/3D/Collision/CollisionObject3DBase.h>
#include <Physics/3D/Collision/SphereCollisionObject.h>

#include <cmath>
const float EulerConstant = std::exp(1.0f);

bool PhysicsObject3DBase::SweepCollisionTo(const Vector3D& position, SweepCollisionResponse3D & response)
{
	//return pEngine->GetCollisionBucketRootFor3D()->SweepCollision(sceneObject3D->GetCollision3D(), position, response);
	return false;
}

bool PhysicsObject3DBase::FastSweepCollision(const Vector3D& position)
{
	/*Vec3D startingPosition = sceneObject3D->GetPosition();
	Vec3D size = sceneObject3D->GetCollision3D()->GetSize() / 2.0;

	float minPositionx = min(startingPosition.x, position.x);
	float maxPositionx = max(startingPosition.x, position.x);

	float minPositiony = min(startingPosition.y, position.y);
	float maxPositiony = max(startingPosition.y, position.y);

	float minPositionz = min(startingPosition.z, position.z);
	float maxPositionz = max(startingPosition.z, position.z);

	sweepCollisionBox->minPos.x = minPositionx - size.x;
	sweepCollisionBox->maxPos.x = maxPositionx + size.x;

	sweepCollisionBox->minPos.y = minPositiony - size.y;
	sweepCollisionBox->maxPos.y = maxPositiony + size.y;

	sweepCollisionBox->minPos.z = minPositionz - size.z;
	sweepCollisionBox->maxPos.z = maxPositionz + size.z;

	CollisionResponse3D unused;
	return pEngine->GetCollisionBucketRootFor3D()->CheckObjectAgainstStaic(sweepCollisionBox, unused);*/

	return false;
}

PhysicsObject3DBase::PhysicsObject3DBase(Component3DBase* component) : gravity(0,-980,0), gravityNormal(0,-1,0), PhysicsObjectBase(component)
{
	sweepCollisionBox = new AABBoxCollisionObject(Vector3D(), Vector3D(), component, CD_Static, AABBOX_COLLISION);
}

PhysicsObject3DBase::~PhysicsObject3DBase()
{
	// Not in charge of deleting Scene or collision objects, thats the engines job
	// We do need to unregister though

	delete sweepCollisionBox;
}

void PhysicsObject3DBase::SetVeloctiy(const Vector3D& Velocity)
{
	this->velocity = Velocity;
}

void PhysicsObject3DBase::SetGravity(const Vector3D& gravity)
{
	this->gravity = gravity;
	gravityNormal = gravity;
	gravityNormal.normalize();
}

void PhysicsObject3DBase::OnCollision(CollisionResponse3D &response)
{
	if (shouldSimulate)
	{
		if (response.collided)
		{
			CollisionObjectBase* collision = response.objectBounds[0];
			CollisionObjectBase* collision2 = response.objectBounds[1];

			PhysicsObject3DBase* other = response.collidedObjects[1];

			affected3DComponent->Translate(response.penetration);

			Vec3D Vel = velocity - other->velocity;
			Vector3D F = (-response.normal * ((1.0f + restitution) * Vel.dot(response.normal))) / (mass + other->mass);
			velocity += F * other->mass * 0.99f;
			// very complicated and realistic isOnGround code. need to probably optmize this somehow later
			if (isOnGround == false)
			{
				Vec3D cross = response.normal.cross(gravity);
				float dot = response.normal.dot(gravity);
				if (cross.getMagnitudeSqr() == 0 && dot > 0)
				{
					Vec3D velocityGravNorm = (velocity + 10 * gravityNormal) * gravityNormal.getAbs();
					if (gravity.dot(velocityGravNorm) >= 0.0f)
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

bool PhysicsObject3DBase::SweepToo(const Vector3D& targetPosition, SweepCollisionResponse3D & response)
{
	if (FastSweepCollision(targetPosition))
	{
		return SweepCollisionTo(targetPosition, response);
	}

	return false;
}

void PhysicsObject3DBase::ApplyForce(const Vector3D& Force)
{
	this->velocity += Force;
	isOnGround = false;
	otherFriction = 1.0;
}

void PhysicsObject3DBase::Update(float deltaTime)
{
	if (shouldSimulate)
	{
		if (isOnGround == false)velocity += gravity * deltaTime;
		else
			velocity *= (friction*otherFriction);
		velocity *= drag;

		if (useSweptCollision)
		{
			Vec3D target = affected3DComponent->GetWorldLocation() + (velocity*deltaTime);
			SweepCollisionResponse3D response;
			if (SweepToo(target, response))
			{
				Vec3D actual = GetScenePos() + (velocity * deltaTime * response.timeHit);
				affected3DComponent->SetWorldLocation(actual);
				if (response.timeHit < 1.0f)
				{
					float invTime = (1.0f - response.timeHit);
					velocity *= invTime;
				}
				OnCollision(response.CollisionResponse3D);

			}
			else
				affected3DComponent->SetWorldLocation(target);
		}
		else
		{
			affected3DComponent->Translate(velocity*deltaTime);
		}
	}
}

const Vector3D& PhysicsObject3DBase::GetVelocity()
{
	return velocity;
}

const Vector3D PhysicsObject3DBase::GetScenePos() { return affected3DComponent->GetWorldLocation(); }
Component3DBase* PhysicsObject3DBase::GetAffected3DComponent() { return affected3DComponent; }