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
	other->SetBoundsBySceneObject();

	switch(other->GetCollisionType())
	{
		case BOX_COLLISION:
		{
			BoxCollisionObject* otherBox = (BoxCollisionObject*)other;
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

Vector3D BoxCollisionObject::GetNormalBetween(CollisionObjectBase * other)
{
	Vec3D diff = other->GetScenePos() - GetScenePos();

	double sx = diff.x > 0 ? 1 : -1;
	double sy = diff.y > 0 ? 1 : -1;
	double sz = diff.z > 0 ? 1 : -1;

	Vec3D normal;

	diff = diff.getAbs();

	if (diff.x > diff.y && diff.x > diff.z) normal = Vec3D(sx, 0, 0);
	else if (diff.y > diff.z) normal = Vec3D(0, sy, 0);
	else normal = Vec3D(0, 0, sz);

	return normal;
}

Vector3D BoxCollisionObject::GetSize()
{
	return Vector3D((maxPos - minPos));
}
