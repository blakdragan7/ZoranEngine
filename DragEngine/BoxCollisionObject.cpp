#include "stdafx.h"
#include "BoxCollisionObject.h"
#include "SphereCollisionObject.h"

BoxCollisionObject::BoxCollisionObject(Vector3D min, Vector3D max, SceneObject* object, unsigned collisionType) : CollisionObjectBase(object,collisionType)
{
	this->minPos = min;
	this->maxPos = max;
}


BoxCollisionObject::~BoxCollisionObject()
{
}

bool BoxCollisionObject::CollidesWith(Vector3D pos)
{
	return (pos.x >= minPos.x && pos.x <= maxPos.x) && \
		   (pos.y >= minPos.y && pos.y <= maxPos.y) && \
		   (pos.z >= minPos.z && pos.z <= maxPos.z);
}

bool BoxCollisionObject::CollidesWith(CollisionObjectBase * other)
{
	Vector3D pos = GetScenePos();

	Vector3D nmip = minPos + pos;
	Vector3D nmap = maxPos + pos;

	switch(other->GetCollisionType())
	{
		case BOX_COLLISION:
		{
			BoxCollisionObject* otherBox = (BoxCollisionObject*)other;

			Vector3D onmip = otherBox->minPos + other->GetScenePos();
			Vector3D onmap = otherBox->maxPos + other->GetScenePos();

			return	(nmip.x <= onmap.x && nmap.x >= onmip.x) && \
				(nmip.y <=onmap.y && nmap.y >= onmip.y) && \
				(nmip.z <= onmap.z && nmap.z >= onmip.z);
		}
		break;
		case SPHERE_COLLISION:
		{
			return other->CollidesWith(GetClosestPointTo(other->GetScenePos()));
		}
		break;
	}
	return false;
}

Vector3D BoxCollisionObject::GetClosestPointTo(Vector3D pos)
{
	Vector3D point;

	point.x = max(this->minPos.x, min(pos.x, this->maxPos.x));
	point.y = max(this->minPos.y, min(pos.y, this->maxPos.y));
	point.z = max(this->minPos.z, min(pos.z, this->maxPos.z));

	return point;
}

Vector3D BoxCollisionObject::GetSize()
{
	return Vector3D((maxPos - minPos));
}
