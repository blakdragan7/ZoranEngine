#include "stdafx.h"
#include "SphereCollisionObject.h"
#include "BoxCollisionObject.h"
#include "SceneObject.h"

SphereCollisionObject::SphereCollisionObject(double radius, SceneObject* object, unsigned collisionType) : CollisionObjectBase(object, collisionType)
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

bool SphereCollisionObject::CollidesWith(Vector3D pos)
{
	SetBoundsBySceneObject();
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

Vector3D SphereCollisionObject::GetNormalBetween(CollisionObjectBase * other)
{
	Vec3D normal = other->GetScenePos() - GetScenePos();
	normal.normalize();
	return normal;
}
