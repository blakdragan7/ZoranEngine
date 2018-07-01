#include "stdafx.h"
#include "SquareColisionObject.h"
#include <Core/2D/SceneObject2D.h>

SquareColisionObject::SquareColisionObject(Vector2D min, Vector2D max, SceneObject2D* object, CollisionDynamics dynamics) : CollisionObject2DBase(object, dynamics, SQUARE_COLLISION)
{
	this->minPos = min;
	this->maxPos = max;

	this->size = maxPos - minPos;
	this->scaledSize = this->size;
}

SquareColisionObject::~SquareColisionObject()
{
}

void SquareColisionObject::SetBoundsBySceneObject()
{
	Vector2D pos = GetScenePos();
	Vector2D scale = GetSceneObject()->GetScale().getAbs();

	scaledSize = size * scale;

	minPos = pos - (scaledSize / 2);
	maxPos = pos + (scaledSize / 2);
}

bool SquareColisionObject::CollidesWith(CollisionObject2DBase* other, CollisionResponse2D& response)
{
	Vec2D otherMin;
	Vec2D otherMax;

	if (other->GetCollisionType() == SQUARE_COLLISION)
	{
		SquareColisionObject* squareOther = (SquareColisionObject*)other;
		otherMin = squareOther->minPos;
		otherMax = squareOther->maxPos;
	}
	else
	{
		// treat the other as a box for now
		Vec2D size = other->GetSize();
		otherMin = other->GetScenePos() - size / 2.0;
		otherMax = other->GetScenePos() + size / 2.0;
	}

	static const Vector2D faces[6] =
	{
		Vector2D(-1,  0), // 'left' face normal (-x direction)
		Vector2D(1,  0), // 'right' face normal (+x direction)
		Vector2D(0, -1), // 'bottom' face normal (-y direction)
		Vector2D(0,  1), // 'top' face normal (+y direction)
	};

	double distances[4] =
	{
		(otherMax.x - minPos.x), // distance of otherBox to face on 'left' side.
		(maxPos.x - otherMin.x), // distance of otherBox to face on 'right' side.
		(otherMax.y - minPos.y), // distance of otherBox to face on 'bottom' side.
		(maxPos.y - otherMin.y), // distance of otherBox to face on 'top' side.
	};

	double penetration = 0;
	Vec2D normal;
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

Vector2D SquareColisionObject::GetClosestPointTo(Vector2D pos)
{
	Vector2D point;

	point.x = max(this->minPos.x, min(pos.x, this->maxPos.x));
	point.y = max(this->minPos.y, min(pos.y, this->maxPos.y));

	return point;
}


Vector2D SquareColisionObject::GetSize()
{
	return Vector2D(maxPos - minPos);
}
