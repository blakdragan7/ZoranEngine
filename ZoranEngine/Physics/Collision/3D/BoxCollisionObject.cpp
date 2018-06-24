#include "stdafx.h"
#include "BoxCollisionObject.h"
#include "SphereCollisionObject.h"
#include "Core/SceneObject.h"
#include "Math/Matrix.hpp"

BoxCollisionObject::BoxCollisionObject(Vector3D min, Vector3D max, SceneObject* object, CollisionDynamics dynamics, unsigned collisionType) : CollisionObjectBase(object, dynamics,collisionType)
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

bool BoxCollisionObject::CollidesWith(CollisionObjectBase * other, CollisionResponse& response)
{
	Vec3D otherMin;
	Vec3D otherMax;

	if (other->GetCollisionType() == BOX_COLLISION)
	{
		BoxCollisionObject* boxOther = (BoxCollisionObject*)other;
		otherMin = boxOther->minPos;
		otherMax = boxOther->maxPos;
	}
	else
	{
		// treat the other as a box for now
		Vec3D size = other->GetSize();
		otherMin = other->GetScenePos() - size / 2.0;
		otherMax = other->GetScenePos() + size / 2.0;
	}

	static const Vector3D faces[6] =
	{
		Vector3D(-1,  0,  0), // 'left' face normal (-x direction)
		Vector3D(1,  0,  0), // 'right' face normal (+x direction)
		Vector3D(0, -1,  0), // 'bottom' face normal (-y direction)
		Vector3D(0,  1,  0), // 'top' face normal (+y direction)
		Vector3D(0,  0, -1), // 'far' face normal (-z direction)
		Vector3D(0,  0,  1), // 'near' face normal (+x direction)
	};

	float distances[6] =
	{
		(otherMax.x - minPos.x), // distance of otherBox to face on 'left' side.
		(maxPos.x - otherMin.x), // distance of otherBox to face on 'right' side.
		(otherMax.y - minPos.y), // distance of otherBox to face on 'bottom' side.
		(maxPos.y - otherMin.y), // distance of otherBox to face on 'top' side.
		(otherMax.z - minPos.z), // distance of otherBox to face on 'far' side.
		(maxPos.z - otherMin.z), // distance of otherBox to face on 'near' side.
	};

	double penetration = 0;
	Vec3D normal;
	int faceIndex = -1;

	for (int i = 0; i < 4; i++)
	{
		// box does not intersect face. So boxes don't intersect at all.
		if (distances[i] < 0.0f)
		{
			response.collided = false;
			return false;
		}
		// face of least intersection depth. That's our candidate.
		if ((i == 0) || (distances[i] < penetration))
		{
			faceIndex = i;
			normal = faces[i];
			penetration = distances[i];
		}
	}

	response.collidedObjects[0] = GetPhysicsObject();
	response.collidedObjects[1] = other->GetPhysicsObject();

	response.objectBounds[0] = this;
	response.objectBounds[1] = other;

	response.collided = true;
	response.normal = normal;
	response.penetration = penetration * -normal;

	return true;
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
