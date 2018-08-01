#include "stdafx.h"
#include "CircleCollision2DObject.h"
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/b2DCollision2DObject.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Math/MathLib.h>

CircleCollision2DObject::CircleCollision2DObject(float radius, SceneObject2D* sceneObject, CollisionDynamics cd) : CollisionObject2DBase(sceneObject,cd,CIRCLE_COLLISION)
{
	this->radius = radius;
}

CircleCollision2DObject::~CircleCollision2DObject()
{
}

void CircleCollision2DObject::SetBoundsBySceneObject()
{
	Vec2D size = GetSceneObject()->GetScale();
  	scaledRadius = radius * max(size.x, size.y);
}

Vector2D CircleCollision2DObject::GetSize()
{
	return Vec2D(scaledRadius, scaledRadius);
}

bool CircleCollision2DObject::CollidesWithNoCollision(CollisionObject2DBase * other)
{
	return false;
}

Collision2D* CircleCollision2DObject::CollidesWith(CollisionObject2DBase * other)
{
	if (other->GetCollisionType() == SQUARE_COLLISION)
	{
		Vec2D pos = GetScenePos();
		Vec2D otherPos = other->GetScenePos();
		Vec2D otherSize = other->GetSize() / 2.0;

		float nearestX = max(otherPos.x - otherSize.x, min(pos.x, otherPos.x + otherSize.x));
		float nearestY = max(otherPos.y - otherSize.y, min(pos.y, otherPos.y + otherSize.y));

		float DeltaX = pos.x - nearestX;
		float DeltaY = pos.y - nearestY;

		if ((DeltaX * DeltaX + DeltaY * DeltaY) < (scaledRadius * scaledRadius))
		{
			Collision2D* collision = new Collision2D();

			collision->collided = true;
			collision->collidedObjects[0] = GetPhysicsObject();
			collision->collidedObjects[1] = other->GetPhysicsObject();
			collision->objects[0] = GetSceneObject();
			collision->objects[1] = other->GetSceneObject();
			collision->objectBounds[0] = this;
			collision->objectBounds[1] = other;
			collision->friction = sqrt(GetSceneObject()->GetPhysics()->GetFriction() * other->GetSceneObject()->GetPhysics()->GetFriction());

			CollisionPoint cp;
			cp.normal = -Vec2D(DeltaX,DeltaY).getNormal();
			cp.pos = Vec2D(nearestX,nearestY);
			cp.separation = Vec2D(DeltaX, DeltaY).magnitude() - scaledRadius;

			collision->AddCollisionPoint(cp);
			return collision;
		}
	}
	else if (other->GetCollisionType() == b2D_2D_COLLISION)
	{
		Vec2D distance = (other->GetScenePos() - GetScenePos());

		Vec2D size = other->GetSize();

		float cs = cosf(other->GetSceneObject()->GetRotation());
		float sn = sinf(other->GetSceneObject()->GetRotation());

		float rotatedx = distance.x * cs + distance.y * sn;
		float rotatedy = distance.x * sn + distance.y * cs;

		Vec2D rotatedDiff(rotatedx,rotatedy);
		Vec2D clampedDiff = MathLib::Clamp(rotatedDiff, -size, size);

		float unrotatedX = (clampedDiff.x * cs - clampedDiff.y * sn) + other->GetScenePos().x;
		float unrotatedY = (-clampedDiff.x * sn - clampedDiff.y * cs) + other->GetScenePos().y;

		Vec2D unrotatedVector(unrotatedX, unrotatedY);

		float magSQR = (unrotatedVector - GetScenePos()).magnitudeSqr();

		if (magSQR <= (scaledRadius * scaledRadius))
		{
			Collision2D* collision = new Collision2D();

			collision->collided = true;
			collision->collidedObjects[0] = GetPhysicsObject();
			collision->collidedObjects[1] = other->GetPhysicsObject();
			collision->objects[0] = GetSceneObject();
			collision->objects[1] = other->GetSceneObject();
			collision->objectBounds[0] = this;
			collision->objectBounds[1] = other;
			collision->friction = sqrt(GetSceneObject()->GetPhysics()->GetFriction() * other->GetSceneObject()->GetPhysics()->GetFriction());

			float mag = sqrtf(magSQR);

			CollisionPoint cp;
			cp.normal = distance.getNormal();
			cp.pos = unrotatedVector;
			cp.separation = (mag - scaledRadius);
			collision->AddCollisionPoint(cp);

			return collision;
		}
	}
	else if (other->GetCollisionType() == CIRCLE_COLLISION)
	{
		Vec2D distance = (other->GetScenePos() - GetScenePos());
		float distanceSqr = distance.magnitudeSqr();
		
		CircleCollision2DObject* otherCircle = (CircleCollision2DObject*)other;
		float combinedRadius = otherCircle->scaledRadius + scaledRadius;
		
		if (distanceSqr <= combinedRadius * combinedRadius)
		{
			Collision2D* collision = new Collision2D();

			collision->collided = true;
			collision->collidedObjects[0] = GetPhysicsObject();
			collision->collidedObjects[1] = other->GetPhysicsObject();
			collision->objects[0] = GetSceneObject();
			collision->objects[1] = other->GetSceneObject();
			collision->objectBounds[0] = this;
			collision->objectBounds[1] = other;
			collision->friction = sqrt(GetSceneObject()->GetPhysics()->GetFriction() * other->GetSceneObject()->GetPhysics()->GetFriction());

			CollisionPoint cp;
			float mag = sqrtf(distanceSqr);
			cp.normal = distance / mag;
			cp.pos = GetScenePos() + (cp.normal * scaledRadius);
			cp.separation = mag- combinedRadius;

			collision->AddCollisionPoint(cp);

			return collision;
		}
	}

	return 0;
}

Vector2D CircleCollision2DObject::GetClosestPointTo(Vector2D pos)
{
	return pos;
}

bool CircleCollision2DObject::SweepCollidesWith(CollisionObject2DBase * other, Vector2D newPosition, SweepCollision2D & response)
{
	return false;
}

bool CircleCollision2DObject::FastSweepCollidesWith(Vector2D newPosition)
{
	return true;
}
