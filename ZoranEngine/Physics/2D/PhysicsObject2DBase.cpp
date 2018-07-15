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
	pEngine->GetCollisionBucketRootFor2D()->SweepCollision(sceneObject2D->GetCollision2D(), position, response);
	return response.CollisionResponse2D.collided;
}

bool PhysicsObject2DBase::FastSweepCollision(Vec2D position)
{
	return sceneObject2D->GetCollision2D()->FastSweepCollidesWith(position);
}

void PhysicsObject2DBase::CheckIfOnGround(struct CollisionResponse2D& response)
{
	// very complicated and realistic isOnGround code. need to probably optmize this somehow later
	//if (isOnGround == false && response.objectBounds[1]->GetCollisionLayer() == COLLISION_LAYER_GROUND)
	//{
	//	double cross = response.penetration.cross(gravity);
	//	double dot = response.penetration.dot(gravity);
	//	if (cross == 0 && dot < 0)
	//	{
	//		Vec2D velocityGravNorm = (velocity /*+ 1 * gravityNormal*/)* gravityNormal.getAbs();
	//		if (MathLib::signum(gravity.x) == MathLib::signum(velocityGravNorm.x) && MathLib::signum(gravity.y) == MathLib::signum(velocityGravNorm.y))
	//		{
	//			isOnGround = true;
	//			velocity += (velocity* gravityNormal);
	//			otherFriction = response.collidedObjects[1]->friction;
	//			calculatedFriction = sqrt(friction*otherFriction);
	//		}
	//	}
	//}
	//else
	//{
	//	isOnGround = false;
	//}
}

PhysicsObject2DBase::PhysicsObject2DBase(SceneObject2D * object) : PhysicsObjectBase(object)
{
	sceneObject2D = object;
	gravity = Vec2D(0, -980);
	gravityNormal = Vec2D(0, -1);
	shouldSimulate = false;
	torque = 0;
	angularVelocity = 0;
}

PhysicsObject2DBase::~PhysicsObject2DBase()
{
	// Not in charge of deleting Scene or collision objects, thats the engines job
	// We do need to unregister though

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

void PhysicsObject2DBase::SetMass(double mass)
{
	PhysicsObjectBase::SetMass(mass);

	Vec2D size = sceneObject2D->GetSize();

	inertia = mass * (size.x*size.x + size.y*size.y) / 12.0;
	invInertia = 1.0 / inertia;
}

void PhysicsObject2DBase::OnCollision(CollisionResponse2D &response)
{
	if (response.collided)
	{
		CollisionObject2DBase* collision = response.objectBounds[0];
		CollisionObject2DBase* collision2 = response.objectBounds[1];

		PhysicsObject2DBase* other = response.collidedObjects[1];

		// apply correction  force
		//sceneObject2D->Translate(response.penetration);
		
		if (shouldSimulate)
		{
			/*Vec2D Vel = response.velocitySnapshot[0] - response.velocitySnapshot[1];
			Vector2D F = (-response.normal.getAbs() * ((1.0 + restitution) * Vel)) / (mass + other->mass);
			velocity += F *other->mass;
			velocity *= (other->friction * friction);*/
			//CheckIfOnGround(response);
		}
	}
}

bool PhysicsObject2DBase::OnSweepCollision(SweepCollisionResponse2D & response,double deltaTime, int currentDepth, int maxDepth)
{
	if (currentDepth >= maxDepth)
	{
		std::string name = response.CollisionResponse2D.collidedObjects[0]->GetSceneObject()->readableName;
		Log(LogLevel_Default,"Error, Object %s exceeded maximum sweep Depth !! \n", name.c_str());
		return false;
	}

	if (shouldSimulate && deltaTime > 1e-9)
	{
		/*CollisionObject2DBase* collision = response.CollisionResponse2D.objectBounds[0];
		CollisionObject2DBase* collision2 = response.CollisionResponse2D.objectBounds[1];

		PhysicsObject2DBase* other = response.CollisionResponse2D.collidedObjects[1];

		Vec2D actual = GetScenePos() + (velocity * deltaTime * response.timeHit);
		sceneObject2D->SetPosition(actual);

		Vec2D Vel = response.CollisionResponse2D.velocitySnapshot[0] - response.CollisionResponse2D.velocitySnapshot[1];
		Vector2D F = (-response.CollisionResponse2D.normal.getAbs() * ((1.0 + restitution) * Vel)) / (mass + other->mass);

		if (collision2->GetDynamics() == CD_Static)
		{
			velocity += (F *(other->mass + mass));
		}
		else
		{
			velocity += (F *other->mass);
		}

		velocity *= (other->friction * friction);
		Vec2D targetPos = GetScenePos() + (velocity * deltaTime * (1.0 - response.timeHit));

		SweepCollisionResponse2D inner_response;

		if (SweepToo(targetPos, inner_response))
		{
			if (OnSweepCollision(inner_response, deltaTime * (1.0 - response.timeHit), ++currentDepth, maxDepth) == false)
				return false;
		}
		else
		{
			sceneObject2D->SetPosition(targetPos);
		}

		CheckIfOnGround(response.CollisionResponse2D);*/

		return true;
	}

	return false;
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

void PhysicsObject2DBase::UpdateVelocities(double deltaTime)
{
	if (shouldSimulate)
	{
		currentDeltaTime = deltaTime;

		if (isOnGround == false)
		{
			velocity += deltaTime * (gravity + invMass * force);
			angularVelocity += deltaTime * invInertia * torque;
		}
		else
		{
			velocity *= calculatedFriction;
		}
		velocity *= drag;

		force.clear();
	}
}

void PhysicsObject2DBase::UpdatePositionsAndRotation(double deltaTime)
{
	if (shouldSimulate)
	{
		currentDeltaTime = deltaTime;

		if (useSweptCollision)
		{
			Vec2D target = sceneObject2D->GetPosition() + ((velocity*deltaTime));

			SweepCollisionResponse2D response;
			if (SweepToo(target, response))
				pEngine->CurrentCollisionFrame2D().sweptCollisions.push_back(response);
			else
				sceneObject2D->SetPosition(target);
		}
		else
		{
			sceneObject2D->Translate(velocity*deltaTime);
		}

		sceneObject2D->Rotate(deltaTime * angularVelocity);
	}
}

Vec2D PhysicsObject2DBase::GetVelocity()
{
	return velocity;
}

Vec2D PhysicsObject2DBase::GetScenePos() { return sceneObject2D->GetPosition(); }
inline SceneObject2D* PhysicsObject2DBase::GetSceneObject() { 
	return sceneObject2D; }