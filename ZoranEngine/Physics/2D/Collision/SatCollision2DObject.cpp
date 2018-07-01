#include "stdafx.h"
#include <string>
#include "SatCollision2DObject.h"

#include <Math/MathLib.h>
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>

#define BOTTOM_LEFT 0
#define TOP_LEFT 1
#define BOTTOM_RIGHT 2
#define TOP_RIGHT 3

bool SatCollision2DObject::TestAgainstOtherSquare(SatCollision2DObject * other, CollisionResponse2D & response)
{
	double cosa = cos(-GetSceneObject()->GetRotationRad());
	double sina = sin(-GetSceneObject()->GetRotationRad());

	double cosb = cos(-other->GetSceneObject()->GetRotationRad());
	double sinb = sin(-other->GetSceneObject()->GetRotationRad());

	Vec2D axis[4] = {
		Vec2D(-cosa, sina),
		Vec2D(cosa, sina),
		Vec2D(-cosb, sinb),
		Vec2D(cosb, sinb)
	};

	for (int axes_index = 0; axes_index < 4; axes_index++)
	{
		double mina = 1000000000;
		double maxa = 0;

		double minb = 1000000000;
		double maxb = 0;

		for (int point_index = 0; point_index < 4; point_index++)
		{
			double currenta = derivedPoints[point_index].dot(axis[axes_index]);
			double currentb = other->derivedPoints[point_index].dot(axis[axes_index]);

			mina = min(currenta, mina);
			maxa = max(currenta, maxa);

			minb = min(currentb, minb);
			maxb = max(currentb, maxb);
		}

		if (maxa < minb || maxb < mina) return false;
	}

	return true;
}

bool SatCollision2DObject::TestAgainstOtherTriagnle(SatCollision2DObject * other, CollisionResponse2D & response)
{
	return false;
}

bool SatCollision2DObject::TestAgainstOtherCircle(SatCollision2DObject * other, CollisionResponse2D & response)
{
	return false;
}

bool SatCollision2DObject::TestAgainstOtherAABBSquare(AABBSquareCollisionObject * other, CollisionResponse2D & response)
{
	double cosa = cos(-GetSceneObject()->GetRotationRad());
	double sina = sin(-GetSceneObject()->GetRotationRad());

	double cosb = 1;
	double sinb = 0;

	Vec2D otherMin = other->GetMinPos();
	Vec2D otherMax = other->GetMaxPos();

	Vec2D otherPoints[4] = {
		otherMin,
		Vec2D(otherMin.x,otherMax.y),
		Vec2D(otherMax.x,otherMin.y),
		otherMax,
	};

	Vec2D axis[4] = {
		Vec2D(-cosa,sina).getNormal(),
		Vec2D(-sina,-cosa).getNormal(),
		Vec2D(-cosb,sinb),
		Vec2D(-sinb,-cosb),
	};

	int normal_index = -1;
	double penetration = std::numeric_limits<double>::infinity();
	bool isNegative = false;

	for (int axes_index = 0; axes_index < 4; axes_index++)
	{
		double mina = std::numeric_limits<double>::infinity();
		double maxa = -std::numeric_limits<double>::infinity();

		double minb = std::numeric_limits<double>::infinity();
		double maxb = -std::numeric_limits<double>::infinity();

		for (int point_index = 0; point_index < 4; point_index++)
		{
			double currenta = derivedPoints[point_index].dot(axis[axes_index]);
			double currentb = otherPoints[point_index].dot(axis[axes_index]);

			mina = min(currenta, mina);
			maxa = max(currenta, maxa);

			minb = min(currentb, minb);
			maxb = max(currentb, maxb);

		}

		if (maxa < minb || maxb < mina) return false;

		double overlap = min(maxa, maxb) - max(mina,minb);

		if ( overlap < penetration )
		{
			isNegative = false;

			penetration = overlap;
			normal_index = axes_index;

			Vec2D dv(mina - minb, maxa - maxb);

			if (dv.dot(axis[axes_index]) < 0)
			{
				isNegative = true;
			}
		}
	}

	response.collided = true;
	response.normal = axis[normal_index];
 	response.penetration = -response.normal * penetration * (isNegative ? -1 : 1);
	response.objectBounds[0] = this;
	response.objectBounds[1] = other;
	response.collidedObjects[0] = GetPhysicsObject();
	response.collidedObjects[1] = other->GetPhysicsObject();

	return true;
}

SatCollision2DObject::SatCollision2DObject(SceneObject2D *object) : CollisionObject2DBase(object,CD_Dynamic,SAT_2D_COLLISION)
{
	polygonType = SATPT_Invalid;
}

SatCollision2DObject::~SatCollision2DObject()
{
}

void SatCollision2DObject::SetAsTriangle(const Vector2D points[3], const Vector2D size)
{
	this->startingPoints[0] = points[0];
	this->startingPoints[1] = points[1];
	this->startingPoints[2] = points[2];

	this->size = size;

	polygonType = SATPT_Triangle;
}

void SatCollision2DObject::SetAsSquare(const Vector2D points[4], const Vector2D size)
{
	this->startingPoints[0] = points[0];
	this->startingPoints[1] = points[1];
	this->startingPoints[2] = points[2];
	this->startingPoints[3] = points[3];

	this->size = size;

	polygonType = SATPT_Square;
}

void SatCollision2DObject::SetAsCircle(double radius)
{
	this->radius = radius;

	polygonType = SATPT_Circle;
}

void SatCollision2DObject::SetBoundsBySceneObject()
{
	switch (polygonType)
	{
	case SATPT_Triangle:
	{
		double sinv = sin(GetSceneObject()->GetRotationRad());
		double cosv = cos(GetSceneObject()->GetRotationRad());

		Vec2D scale = GetSceneObject()->GetScale();
		Vec2D pos = GetScenePos();

		scaledSize = scale * size;

		Vec2D tempPoinnts[3] = {
			startingPoints[0],
			startingPoints[1],
			startingPoints[2],
		};

		tempPoinnts[0] = (startingPoints[0] * scale);
		tempPoinnts[1] = (startingPoints[1] * scale);
		tempPoinnts[2] = (startingPoints[2] * scale);

		derivedPoints[0].x = (cosv * tempPoinnts[0].x) - (sinv * tempPoinnts[0].y);
		derivedPoints[0].y = (sinv * tempPoinnts[0].x) + (cosv * tempPoinnts[0].y);

		derivedPoints[1].x = (cosv * tempPoinnts[1].x) - (sinv * tempPoinnts[1].y);
		derivedPoints[1].y = (sinv * tempPoinnts[1].x) + (cosv * tempPoinnts[1].y);

		derivedPoints[2].x = (cosv * tempPoinnts[2].x) - (sinv * tempPoinnts[2].y);
		derivedPoints[2].y = (sinv * tempPoinnts[2].x) + (cosv * tempPoinnts[2].y);

		derivedPoints[0] += pos;
		derivedPoints[1] += pos;
		derivedPoints[2] += pos;

	}
		break;
	case SATPT_Square:
	{
		double sinv = sin(GetSceneObject()->GetRotationRad());
		double cosv = cos(GetSceneObject()->GetRotationRad());

		Vec2D scale = GetSceneObject()->GetScale();
		Vec2D pos = GetScenePos();

		scaledSize = scale * size;

		derivedPoints[0].x = (cosv * startingPoints[0].x) - (sinv * startingPoints[0].y);
		derivedPoints[0].y = (sinv * startingPoints[0].x) + (cosv * startingPoints[0].y);

		derivedPoints[1].x = (cosv * startingPoints[1].x) - (sinv * startingPoints[1].y);
		derivedPoints[1].y = (sinv * startingPoints[1].x) + (cosv * startingPoints[1].y);

		derivedPoints[2].x = (cosv * startingPoints[2].x) - (sinv * startingPoints[2].y);
		derivedPoints[2].y = (sinv * startingPoints[2].x) + (cosv * startingPoints[2].y);

		derivedPoints[3].x = (cosv * startingPoints[3].x) - (sinv * startingPoints[3].y);
		derivedPoints[3].y = (sinv * startingPoints[3].x) + (cosv * startingPoints[3].y);

		derivedPoints[0] = (derivedPoints[0] * scale) + pos;
		derivedPoints[1] = (derivedPoints[1] * scale) + pos;
		derivedPoints[2] = (derivedPoints[2] * scale) + pos;
		derivedPoints[3] = (derivedPoints[3] * scale) + pos;
	}
		break;
	case SATPT_Circle:
	{
		Vec2D scale = GetSceneObject()->GetScale();
		derivedRadius = radius * max(scale.x, scale.y);
	}
		break;
	case SATPT_Invalid:
	default:
		throw std::exception("SatCollision2DObject::SetBoundsBySceneObject Trying to Update Without setting type first !!");
		break;
	}
}

Vector2D SatCollision2DObject::GetSize()
{
	return GetSceneObject()->GetScale();
}

bool SatCollision2DObject::CollidesWith(CollisionObject2DBase * other, CollisionResponse2D & response)
{
	switch (other->GetCollisionType())
	{
	case SQUARE_COLLISION:
	{
		return TestAgainstOtherAABBSquare((AABBSquareCollisionObject*)other, response);
	}
		break;
	case SAT_2D_COLLISION:
	{
		SatCollision2DObject* satOther = (SatCollision2DObject*)other;
		switch (satOther->polygonType)
		{
		case SATPT_Triangle:
			return TestAgainstOtherTriagnle(satOther, response);
		case SATPT_Square:
			return TestAgainstOtherSquare(satOther, response);
		case SATPT_Circle:
			return TestAgainstOtherCircle(satOther, response);
		case SATPT_Invalid:
		default:
			return false;
		}
	}
		break;
	default:
		return false;
	}
}

Vector2D SatCollision2DObject::GetClosestPointTo(Vector2D pos)
{
	return pos;
}
