#include "stdafx.h"
#include "PhysicsObject2DBase.h"

#include <Core/ZoranEngine.h>
#include <Physics/PhysicsEngine.h>
#include <Core/2D/Components/Component2DBase.h>
#include <Physics/2D/Collision/CollisionBucket2DBase.h>
#include <Physics/2D/Collision/CollisionObject2DBase.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>

#include <Math/MathHelpers.h>
#include <Math/MathLib.h>
#include <cmath>

const float EulerConstant = std::exp(1.0f);

bool PhysicsObject2DBase::SweepCollisionTo(Vec2D position, SweepCollision2D & response)
{
	/*pEngine->GetCollisionBucketRootFor2D()->SweepCollision(sceneObject2D->GetCollision2D(), position, response);
	return response.Collision2D.collided;*/
	return false;
}

bool PhysicsObject2DBase::FastSweepCollision(Vec2D position)
{
	//return sceneObject2D->GetCollision2D()->FastSweepCollidesWith(position);
	return false;
}

void PhysicsObject2DBase::CheckIfOnGround(class Collision2D& response)
{
	// very complicated and realistic isOnGround code. need to probably optmize this somehow later
	//if (isOnGround == false && response.objectBounds[1]->GetCollisionLayer() == COLLISION_LAYER_GROUND)
	//{
	//	float cross = response.penetration.cross(gravity);
	//	float dot = response.penetration.dot(gravity);
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

PhysicsObject2DBase::PhysicsObject2DBase(Component2DBase* component) : affected2DComponent(component), PhysicsObjectBase(component)
{
	gravity = Vec2D(0, -980);
	gravityNormal = Vec2D(0, -1);
	shouldSimulate = false;
	torque = 0;
}

PhysicsObject2DBase::~PhysicsObject2DBase()
{
	// Not in charge of deleting Scene or collision objects, thats the engines job
	// We do need to unregister though

}

void PhysicsObject2DBase::SetVeloctiy(const Vector2D& Velocity)
{
	this->velocity = Velocity;
}

void PhysicsObject2DBase::SetAngularVeloctiy(float Velocity)
{
	angularVelocity = Velocity;
}

void PhysicsObject2DBase::SetGravity(const Vector2D& gravity)
{
	this->gravity = gravity;
	gravityNormal = gravity;
	gravityNormal.normalize();
}

void PhysicsObject2DBase::SetMass(float mass)
{
 	PhysicsObjectBase::SetMass(mass);

	ReCalculateInertia();
}

void PhysicsObject2DBase::ReCalculateInertia()
{
	Vec2D size = affected2DComponent->GetSize();

 	if (mass < FLT_MAX && mass != 0)
	{
		inertia = mass * (size.x*size.x + size.y*size.y) / 12.0f;
		invInertia = 1.0f / inertia;
	}
	else
	{
		inertia = FLT_MAX;
		invInertia = 0;
	}
}

void PhysicsObject2DBase::OnCollision(Collision2D &response)
{
	if (response.collided)
	{
		CollisionObject2DBase* collision = response.objectBounds[0];
		CollisionObject2DBase* collision2 = response.objectBounds[1];

		PhysicsObject2DBase* other = response.collidedObjects[1];
		/*for (auto collision : response.collisionPoints)
		{
			sceneObject2D->Translate(-collision.normal * collision.separation);
		}*/
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

bool PhysicsObject2DBase::OnSweepCollision(SweepCollision2D & response,float deltaTime, int currentDepth, int maxDepth)
{
	/*if (currentDepth >= maxDepth)
	{
		std::string name = *response.Collision2D.collidedObjects[0]->GetSceneObject()->readableName;
		Log(LogLevel_Error,"Error, Object %s exceeded maximum sweep Depth !! \n", name.c_str());
		return false;
	}*/

	if (shouldSimulate && deltaTime > 1e-9)
	{
		/*CollisionObject2DBase* collision = response.Collision2D.objectBounds[0];
		CollisionObject2DBase* collision2 = response.Collision2D.objectBounds[1];

		PhysicsObject2DBase* other = response.Collision2D.collidedObjects[1];

		Vec2D actual = GetScenePos() + (velocity * deltaTime * response.timeHit);
		sceneObject2D->SetPosition(actual);

		Vec2D Vel = response.Collision2D.velocitySnapshot[0] - response.Collision2D.velocitySnapshot[1];
		Vector2D F = (-response.Collision2D.normal.getAbs() * ((1.0 + restitution) * Vel)) / (mass + other->mass);

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

		SweepCollision2D inner_response;

		if (SweepToo(targetPos, inner_response))
		{
			if (OnSweepCollision(inner_response, deltaTime * (1.0 - response.timeHit), ++currentDepth, maxDepth) == false)
				return false;
		}
		else
		{
			sceneObject2D->SetPosition(targetPos);
		}

		CheckIfOnGround(response.Collision2D);*/

		return true;
	}

	return false;
}

bool PhysicsObject2DBase::SweepToo(Vec2D targetPosition, SweepCollision2D & response)
{
	if (FastSweepCollision(targetPosition))
	{
		return SweepCollisionTo(targetPosition, response);
	}

	return false;
}

void PhysicsObject2DBase::ApplyForce(const Vector2D& Force)
{
	this->force += Force;
	isOnGround = false;
}

void PhysicsObject2DBase::UpdateVelocities(float deltaTime)
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
		torque = 0;
	}
}

void PhysicsObject2DBase::UpdatePositionsAndRotation(float deltaTime)
{
	if (shouldSimulate)
	{
		currentDeltaTime = deltaTime;

		if (useSweptCollision)
		{
			Vec2D target = affected2DComponent->GetWorldLocation() + ((velocity*deltaTime));

			SweepCollision2D response;
			if (SweepToo(target, response))
				pEngine->CurrentCollisionFrame2D().sweptCollisions.push_back(response);
			else
				affected2DComponent->SetWorldLocation(target);
		}
		else
		{
			affected2DComponent->Translate(velocity*deltaTime);
		}

		affected2DComponent->Rotate(deltaTime * -angularVelocity);
	
		// optimization for calculating model matrix only once
		//sceneObject2D->PreCaclModel();
	}
}

void PhysicsObject2DBase::ApplyImpulseToVelocity(const Vector2D& impulse)
{
	velocity += invMass * impulse;
}

void PhysicsObject2DBase::ApplyImpulseToAngularVelocity(float impulse)
{
	angularVelocity += invInertia * impulse;
}

const Vec2D& PhysicsObject2DBase::GetVelocity()
{
	return velocity;
}

float PhysicsObject2DBase::GetAngularVelocity()
{
	return angularVelocity;
}

const Vec2D& PhysicsObject2DBase::GetScenePos() { return affected2DComponent->GetWorldLocation(); }
