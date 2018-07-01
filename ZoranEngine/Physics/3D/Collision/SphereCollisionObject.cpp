#include "stdafx.h"
#include "SphereCollisionObject.h"
#include "AABBoxCollisionObject.h"
#include "Core/SceneObject.h"
#include <Core/3D/SceneObject3D.h>

SphereCollisionObject::SphereCollisionObject(double radius, SceneObject3D* object, CollisionDynamics dynamics, unsigned collisionType) : CollisionObject3DBase(object, dynamics, collisionType)
{
	this->radius = radius;
	this->radiusSqr = radius * radius;
	scaledRadius = 0;
}

SphereCollisionObject::~SphereCollisionObject()
{
}

void SphereCollisionObject::SetBoundsBySceneObject()
{
	Vec3D scale = GetSceneObject()->GetScale().getAbs();

	scaledRadius = radius * max(scale.x, max(scale.y, scale.z));
	radiusSqr = scaledRadius * scaledRadius;
}

bool SphereCollisionObject::CollidesWith(CollisionObject3DBase * other, CollisionResponse3D& response)
{
	switch(other->GetCollisionType())
	{
		case SPHERE_COLLISION:
		{
			SphereCollisionObject* object = (SphereCollisionObject*)other;

			double radiusSqrSum = radiusSqr + object->radiusSqr;
			double distanceSqr = (GetScenePos() - other->GetScenePos()).getMagnitudeSqr();
			
			bool collided = distanceSqr <= radiusSqrSum;
			response.collided = collided;

			if (collided)
			{
				response.collidedObjects[0] = GetPhysicsObject();
				response.collidedObjects[1] = other->GetPhysicsObject();

				response.objectBounds[0] = this;
				response.objectBounds[1] = other;

				response.normal = (GetScenePos() - other->GetScenePos());
				response.normal.normalize();
				response.penetration = (radiusSqrSum - distanceSqr) * -response.normal;
			}
		}
		break;
		case AABBOX_COLLISION:
		{
			return other->CollidesWith(this,response);
		}
		break;
	}

	return response.collided;
}

Vector3D SphereCollisionObject::GetClosestPointTo(Vector3D pos)
{
	// specifically not doing max distance,radius
	// because this is specifcally for collision and that could mess up collision checking

	Vec3D myPos = GetScenePos();
	Vec3D direction = (pos - myPos);
	direction.normalize();

	return myPos + (direction * radius);
}

Vector3D SphereCollisionObject::GetSize()
{
	return Vector3D(radius,radius,radius);
}
