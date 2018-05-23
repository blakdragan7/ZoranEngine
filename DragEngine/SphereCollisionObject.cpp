#include "stdafx.h"
#include "SphereCollisionObject.h"
#include "BoxCollisionObject.h"

SphereCollisionObject::SphereCollisionObject(double radius, SceneObject* object, unsigned collisionType) : CollisionObjectBase(object, collisionType)
{
	this->radius = radius;
	this->radiusSqr = radius * radius;
}

SphereCollisionObject::~SphereCollisionObject()
{
}

bool SphereCollisionObject::CollidesWith(Vector3D pos)
{
	double distanceSqr = (pos - GetScenePos()).getMagnitudeSqr();
	return distanceSqr <= radiusSqr;
}

bool SphereCollisionObject::CollidesWith(CollisionObjectBase * other)
{
	switch(other->GetCollisionType()) 
	{
		case SPHERE_COLLISION:
		{
			SphereCollisionObject* object = (SphereCollisionObject*)other;

			double radiusSqrSum = radiusSqr + object->radiusSqr;
			double distanceSqr = (GetScenePos() - other->GetScenePos()).getMagnitudeSqr();
			return distanceSqr <= radiusSqrSum;
		}
		break;
		case BOX_COLLISION:
		{
			return CollidesWith(other->GetClosestPointTo(GetScenePos()));
		}
		break;
	}

	return false;
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
