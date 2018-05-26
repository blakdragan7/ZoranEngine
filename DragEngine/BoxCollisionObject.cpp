#include "stdafx.h"
#include "BoxCollisionObject.h"
#include "SphereCollisionObject.h"
#include "SceneObject.h"
#include "Matrix.hpp"

BoxCollisionObject::BoxCollisionObject(Vector3D min, Vector3D max, SceneObject* object, unsigned collisionType) : CollisionObjectBase(object,collisionType)
{
	this->minPos = min;
	this->maxPos = max;

	this->size = maxPos - minPos;
	this->scaledSize = this->size;
}


BoxCollisionObject::~BoxCollisionObject()
{
}

void BoxCollisionObject::SetBoundsBySceneObject()
{
	Vector3D pos = GetScenePos();
	Vector3D scale = GetSceneObject()->GetScale().getAbs();

	scaledSize = size * scale;

	minPos = pos - (scaledSize / 2);
	maxPos = pos + (scaledSize / 2);
}

bool BoxCollisionObject::CollidesWith(Vector3D pos)
{
	return (pos.x >= minPos.x && pos.x <= maxPos.x) && \
		   (pos.y >= minPos.y && pos.y <= maxPos.y) && \
		   (pos.z >= minPos.z && pos.z <= maxPos.z);
}

bool BoxCollisionObject::CollidesWith(CollisionObjectBase * other)
{
	SetBoundsBySceneObject();

	switch(other->GetCollisionType())
	{
		case BOX_COLLISION:
		{
			BoxCollisionObject* otherBox = (BoxCollisionObject*)other;
			otherBox->SetBoundsBySceneObject();
			return	(minPos.x <= otherBox->maxPos.x && maxPos.x >= otherBox->minPos.x) && \
				(minPos.y <= otherBox->maxPos.y && maxPos.y >= otherBox->minPos.y) && \
				(minPos.z <= otherBox->maxPos.z && maxPos.z >= otherBox->minPos.z);
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
