#include "stdafx.h"
#include "CircleCollision2DObject.h"
#include <Physics/2D/PhysicsObject2DBase.h>
#include <Physics/2D/Collision/b2DCollision2DObject.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Rendering/RenderedObjectBase.h>
#include <Math/MathLib.h>

#include <Core/2D/Components/RigidBody2DComponent.h>

CircleCollision2DObject::CircleCollision2DObject(float radius, Component2DBase* component, CollisionDynamics cd) : CollisionObject2DBase(component,cd,CIRCLE_COLLISION)
{
	this->radius = radius;
}

CircleCollision2DObject::~CircleCollision2DObject()
{
}

void CircleCollision2DObject::SetBoundsBySceneObject()
{
	Vec2D size = GetAffectedComponent()->GetSize();
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
	SetBoundsBySceneObject();
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
			collision->objects[0] = GetAffectedComponent();
			collision->objects[1] = other->GetAffectedComponent();
			collision->objectBounds[0] = this;
			collision->objectBounds[1] = other;
			collision->friction = sqrt(GetPhysicsObject()->GetFriction() * other->GetPhysicsObject()->GetFriction());


			CollisionPoint cp;
			
			if (DeltaX == 0 && DeltaY == 0)
			{
				unsigned index = 0;
				Vec2D test[4] = {
					Vec2D(nearestX, otherPos.y + otherSize.y),
					Vec2D(nearestX, otherPos.y - otherSize.y),
					Vec2D(otherPos.x - otherSize.x, nearestY),
					Vec2D(otherPos.x + otherSize.x, nearestY)
				};

				Vec2D axes[4] = 
				{
					Vec2D(0,1),
					Vec2D(0,-1),
					Vec2D(-1,0),
					Vec2D(1,0)
				};

				float min = std::numeric_limits<float>::infinity();

				for (unsigned i=0; i < 4; ++i)
				{
					float c = pos.distanceSquared(test[i]);
					if (c < min)
					{
						min = c;
						index = i;
					}
				}

				cp.normal = -axes[index];
			}
			else 
				cp.normal = -Vec2D(DeltaX,DeltaY).getNormal();
			cp.pos = Vec2D(nearestX,nearestY);
			cp.separation = Vec2D(DeltaX, DeltaY).magnitude() - scaledRadius;

			collision->AddCollisionPoint(cp);
			return collision;
		}
	}
	else if (other->GetCollisionType() == b2D_2D_COLLISION)
	{
		Vec2D distancel = GetScenePos() - other->GetScenePos();

		Vec2D size = other->GetSize();

		float cs = cosf(other->GetAffectedComponent()->GetRotation());
		float sn = sinf(other->GetAffectedComponent()->GetRotation());

		float rotatedx = distancel.x * cs + distancel.y * sn;
		float rotatedy = -distancel.x * sn + distancel.y * cs;

		Vec2D rotatedDiff(rotatedx,rotatedy);
		Vec2D clampedDiff = MathLib::Clamp(rotatedDiff, -size, size);

		float unrotatedX = (clampedDiff.x * cs - clampedDiff.y * sn);
		float unrotatedY = (clampedDiff.x * sn + clampedDiff.y * cs);

		float globalX = unrotatedX + other->GetScenePos().x;
		float globalY = unrotatedY + other->GetScenePos().y;

		Vec2D unrotatedVector(globalX, globalY);

		Vec2D distance = (unrotatedVector - GetScenePos());

		float magSQR = distance.magnitudeSqr();

		if (magSQR <= (scaledRadius * scaledRadius))
		{
			Collision2D* collision = new Collision2D();

			collision->collided = true;
			collision->collidedObjects[0] = GetPhysicsObject();
			collision->collidedObjects[1] = other->GetPhysicsObject();
			collision->objects[0] = GetAffectedComponent();
			collision->objects[1] = other->GetAffectedComponent();
			collision->objectBounds[0] = this;
			collision->objectBounds[1] = other;
			collision->friction = sqrt(GetPhysicsObject()->GetFriction() * other->GetPhysicsObject()->GetFriction());


			CollisionPoint cp;

			Vec2D otherPos = other->GetScenePos();
			Vec2D otherSize = other->GetSize();

			if (distance.x == 0 && distance.y == 0)
			{
				unsigned index = 0;
				Vec2D test[4] = {
					Vec2D(unrotatedX, otherPos.y + otherSize.y),
					Vec2D(unrotatedX, otherPos.y - otherSize.y),
					Vec2D(otherPos.x - otherSize.x, unrotatedY),
					Vec2D(otherPos.x + otherSize.x, unrotatedY)
				};

				Vec2D axes[4] =
				{
					Vec2D(0,1),
					Vec2D(0,-1),
					Vec2D(-1,0),
					Vec2D(1,0)
				};

				float min = std::numeric_limits<float>::infinity();

				Vec2D pos = GetScenePos();

				for (unsigned i = 0; i < 4; ++i)
				{
					float c = pos.distanceSquared(test[i]);
					if (c < min)
					{
						min = c;
						index = i;
					}
				}

				cp.normal = -axes[index];
			}
			else
				cp.normal = -Vec2D(distance.x, distance.y).getNormal();
			cp.pos = Vec2D(unrotatedX, unrotatedY);
			cp.separation = Vec2D(distance.x, distance.y).magnitude() - scaledRadius;

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
			collision->objects[0] = GetAffectedComponent();
			collision->objects[1] = other->GetAffectedComponent();
			collision->objectBounds[0] = this;
			collision->objectBounds[1] = other;
			collision->friction = sqrt(GetPhysicsObject()->GetFriction() * other->GetPhysicsObject()->GetFriction());

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
