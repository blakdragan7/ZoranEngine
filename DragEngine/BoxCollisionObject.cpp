#include "stdafx.h"
#include "BoxCollisionObject.h"


BoxCollisionObject::BoxCollisionObject(Vector3D min, Vector3D max, SceneObject* object) : PhysicsObjectBase(object)
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

bool BoxCollisionObject::CollidesWith(PhysicsObjectBase * other)
{
	if (BoxCollisionObject* otherBox = dynamic_cast<BoxCollisionObject*>(other))
	{
		return	(minPos.x <= otherBox->maxPos.x && maxPos.x >= otherBox->minPos.x) && \
				(minPos.y <= otherBox->maxPos.y && maxPos.y >= otherBox->minPos.y) && \
				(minPos.z <= otherBox->maxPos.z && maxPos.z >= otherBox->minPos.z);
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
