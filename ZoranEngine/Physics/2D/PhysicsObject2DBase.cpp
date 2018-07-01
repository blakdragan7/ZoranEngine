#include "stdafx.h"
#include "PhysicsObject2DBase.h"
#include "Core/2D/SceneObject2D.h"
#include "Core/ZoranEngine.h"
#include <Physics/PhysicsEngine.h>
#include "Physics/2D/Collision/CollisionBucket2DBase.h"
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include "Math/MathHelpers.h"
#include <Math/MathLib.h>
#include <cmath>
const double EulerConstant = std::exp(1.0);

bool PhysicsObject2DBase::SweepCollisionTo(Vec2D position, SweepCollisionResponse2D & response)
{
	return pEngine->GetCollisionBucketRootFor2D()->SweepCollision(sceneObject2D->GetCollision2D(), position, response);
}

bool PhysicsObject2DBase::FastSweepCollision(Vec2D position)
{
	Vec2D startingPosition = sceneObject2D->GetPosition();
	Vec2D size = sceneObject2D->GetCollision2D()->GetSize() / 2.0;

	double minPositionx = min(startingPosition.x, position.x);
	double maxPositionx = max(startingPosition.x, position.x);

	double minPositiony = min(startingPosition.y, position.y);
	double maxPositiony = max(startingPosition.y, position.y);

	sweepCollisionSquare->minPos.x = minPositionx - size.x;
	sweepCollisionSquare->maxPos.x = maxPositionx + size.x;
				  
	sweepCollisionSquare->minPos.y = minPositiony - size.y;
	sweepCollisionSquare->maxPos.y = maxPositiony + size.y;

	CollisionResponse2D unused;
	return pEngine->GetCollisionBucketRootFor2D()->CheckObjectAgainstStaic(sweepCollisionSquare, unused);
}

PhysicsObject2DBase::PhysicsObject2DBase(SceneObject2D * object) : PhysicsObjectBase(object)
{
	sceneObject2D = object;
	gravity = Vec2D(0, -980);
	gravityNormal = Vec2D(0, -1);

	sweepCollisionSquare = new AABBSquareCollisionObject(Vec2D(), Vec2D(), object, CD_Static);
}

PhysicsObject2DBase::~PhysicsObject2DBase()
{
	// Not in charge of deleting Scene or collision objects, thats the engines job
	// We do need to unregister though

	delete sweepCollisionSquare;
}

void PhysicsObject2DBase::SetVeloctiy(Vec2D Velocity)
{
	this->velocity = Velocity;
}

void PhysicsObject2DBase::SetGravity(Vector2D gravity)
{
	this->gravity = gravity;
	gravityNormal = gravity;
	gravityNormal.normalize();
}

void PhysicsObject2DBase::OnCollision(CollisionResponse2D &response)
{
	if (shouldSimulate)
	{
		if (response.collided)
		{
			CollisionObject2DBase* collision = response.objectBounds[0];
			CollisionObject2DBase* collision2 = response.objectBounds[1];

			PhysicsObject2DBase* other = response.collidedObjects[1];

			sceneObject2D->Translate(response.penetration);

			Vec2D Vel = velocity - other->velocity;
			Vector2D F = (response.normal * ((1.0 + restitution) * Vel)) / (mass + other->mass);
			velocity += F *other->mass;
			velocity *= (other->friction * friction);
			// very complicated and realistic isOnGround code. need to probably optmize this somehow later
			if (isOnGround == false && collision2->GetDynamics() == CD_Static)
			{
				double cross = response.penetration.cross(gravity);
				double dot = response.penetration.dot(gravity);
				if (cross == 0 && dot > 0)
				{
					Vec2D velocityGravNorm = (velocity + 10 * gravityNormal) * gravityNormal.getAbs();
					if (MathLib::signum(gravity.x) == MathLib::signum(velocityGravNorm.x) && MathLib::signum(gravity.y) == MathLib::signum(velocityGravNorm.y))
					{
						isOnGround = true;
						velocity += (velocity* gravityNormal);
						otherFriction = other->friction;
					}
				}
			}
		}
	}
}

bool PhysicsObject2DBase::SweepToo(Vec2D targetPosition, SweepCollisionResponse2D & response)
{
	if (FastSweepCollision(targetPosition))
	{
		return SweepCollisionTo(targetPosition, response);
	}

	return false;
}

void PhysicsObject2DBase::ApplyForce(Vec2D Force)
{
	this->velocity += Force;
	isOnGround = false;
	otherFriction = 1.0;
}

void PhysicsObject2DBase::Update(double deltaTime)
{
	if (shouldSimulate)
	{
		if (isOnGround == false)velocity += gravity * deltaTime;
		else
			velocity *= (friction*otherFriction);
		velocity *= drag;

		if (useSweptCollision)
		{
			Vec2D target = sceneObject2D->GetPosition() + (velocity*deltaTime);
			SweepCollisionResponse2D response;
			if (SweepToo(target, response))
			{
				Vec2D actual = GetScenePos() + (velocity * deltaTime * response.timeHit);
				sceneObject2D->SetPosition(actual);
				if (response.timeHit < 1.0)
				{
					double invTime = (1.0 - response.timeHit);
					velocity *= invTime;
				}
				OnCollision(response.CollisionResponse2D);

			}
			else
				sceneObject2D->SetPosition(target);
		}
		else
		{
			sceneObject2D->Translate(velocity*deltaTime);
		}
	}
}

Vec2D PhysicsObject2DBase::GetVelocity()
{
	return velocity;
}

Vec2D PhysicsObject2DBase::GetScenePos() { return sceneObject2D->GetPosition(); }
SceneObject2D* PhysicsObject2DBase::GetSceneObject() { return sceneObject2D; }