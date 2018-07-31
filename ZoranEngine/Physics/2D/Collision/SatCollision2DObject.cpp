#include "stdafx.h"
#include <string>
#include "SatCollision2DObject.h"

#include <Math/MathLib.h>
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/2D/PhysicsObject2DBase.h>

#include <algorithm>

char SatCollision2DObject::EdgeFromNormal(Vec2D Normal)
{
	return 0;
}

bool SatCollision2DObject::TestAgainstOtherSquare(SatCollision2DObject * other, Collision2D * response)
{
	float cosa = cosf(-GetSceneObject()->GetRotationRad());
	float sina = sinf(-GetSceneObject()->GetRotationRad());

	float cosb = cosf(-other->GetSceneObject()->GetRotationRad());
	float sinb = sinf(-other->GetSceneObject()->GetRotationRad());

	Vec2D axis[4] = {
		Vec2D(-cosa,sina).getNormal(),
		Vec2D(-sina,-cosa).getNormal(),
		Vec2D(-cosb,sinb).getNormal(),
		Vec2D(-sinb,-cosb).getNormal(),
	};

	int normal_index = -1;
	float penetration = std::numeric_limits<float>::infinity();
	bool isNegative = false;

	for (int axes_index = 0; axes_index < 4; axes_index++)
	{
		float mina = std::numeric_limits<float>::infinity();
		float maxa = -std::numeric_limits<float>::infinity();

		float minb = std::numeric_limits<float>::infinity();
		float maxb = -std::numeric_limits<float>::infinity();

		for (int point_index = 0; point_index < 4; point_index++)
		{
			float currenta = derivedPoints[point_index].dot(axis[axes_index]);
			float currentb = other->derivedPoints[point_index].dot(axis[axes_index]);

			mina = min(currenta, mina);
			maxa = max(currenta, maxa);

			minb = min(currentb, minb);
			maxb = max(currentb, maxb);
		}

		if (maxa < minb || maxb < mina) return false;

		float overlap = min(maxa, maxb) - max(mina, minb);

		if (overlap < penetration)
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

	Vec2D normal = axis[normal_index] * (isNegative ? -1.0f : 1.0f);

	CollisionPoint CollisionPointA[2];

	int numPoints = FindCollisionPoints(CollisionPointA, normal, other->derivedPoints);

	if (numPoints > 0)
	{
		CollisionPointA[0].separation = penetration * (isNegative ? -1 : 1);;
		response->AddCollisionPoint(CollisionPointA[0]);
	}
	if (numPoints > 1)
	{
		CollisionPointA[1].separation = penetration * (isNegative ? -1 : 1);;
		response->AddCollisionPoint(CollisionPointA[1]);
	}

	response->objects[0] = GetSceneObject();
	response->objects[1] = other->GetSceneObject();
	response->collided = true;
	response->objectBounds[0] = this;
	response->objectBounds[1] = other;
	response->collidedObjects[0] = GetPhysicsObject();
	response->collidedObjects[1] = other->GetPhysicsObject();
	response->friction = sqrt(GetSceneObject()->GetPhysics()->GetFriction() * other->GetSceneObject()->GetPhysics()->GetFriction());

	return true;
}

bool SatCollision2DObject::TestAgainstOtherTriagnle(SatCollision2DObject * other, Collision2D * response)
{
	return false;
}

bool SatCollision2DObject::TestAgainstOtherCircle(SatCollision2DObject * other, Collision2D * response)
{
	return false;
}

bool SatCollision2DObject::TestAgainstOtherAABBSquare(AABBSquareCollisionObject * other, Collision2D * response)
{
	float cosa = cosf(-GetSceneObject()->GetRotationRad());
	float sina = sinf(-GetSceneObject()->GetRotationRad());

	float cosb = 1;
	float sinb = 0;

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
	float penetration = std::numeric_limits<float>::infinity();
	bool isNegative = false;

	for (int axes_index = 0; axes_index < 4; axes_index++)
	{
		float mina = std::numeric_limits<float>::infinity();
		float maxa = -std::numeric_limits<float>::infinity();

		float minb = std::numeric_limits<float>::infinity();
		float maxb = -std::numeric_limits<float>::infinity();

		for (int point_index = 0; point_index < 4; point_index++)
		{
			float currenta = derivedPoints[point_index].dot(axis[axes_index]);
			float currentb = otherPoints[point_index].dot(axis[axes_index]);

			mina = min(currenta, mina);
			maxa = max(currenta, maxa);

			minb = min(currentb, minb);
			maxb = max(currentb, maxb);

		}

		if (maxa < minb || maxb < mina) return false;

		float overlap = min(maxa, maxb) - max(mina,minb);

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

	Vec2D normal = axis[normal_index] * (isNegative ? -1.0f : 1.0f);
	
	CollisionPoint CollisionPointA[2];

	int numPoints = FindCollisionPoints(CollisionPointA,normal, otherPoints);

	if (numPoints > 0)
	{
		CollisionPointA[0].separation = penetration * (isNegative ? -1 : 1);;
		response->AddCollisionPoint(CollisionPointA[0]);
	}
	if (numPoints > 1)
	{
		CollisionPointA[1].separation = penetration * (isNegative ? -1 : 1);;
		response->AddCollisionPoint(CollisionPointA[1]);
	}

	response->objects[0] = GetSceneObject();
	response->objects[1] = other->GetSceneObject();
	response->collided = true;
	response->objectBounds[0] = this;
	response->objectBounds[1] = other;
	response->collidedObjects[0] = GetPhysicsObject();
	response->collidedObjects[1] = other->GetPhysicsObject();
	response->friction = sqrt(GetSceneObject()->GetPhysics()->GetFriction() * other->GetSceneObject()->GetPhysics()->GetFriction());

	return true;
}

bool SatCollision2DObject::SweepTestAgainstOtherSquare(SatCollision2DObject* other, SweepCollision2D & response)
{
	return 1.0;
}

bool SatCollision2DObject::SweepTestAgainstOtherTriagnle(SatCollision2DObject* other, SweepCollision2D & response)
{
	return 1.0;
}

bool SatCollision2DObject::SweepTestAgainstOtherCircle(SatCollision2DObject* other, SweepCollision2D & response)
{
	return 1.0;
}

bool SatCollision2DObject::SweepTestAgainstOtherAABBSquare(AABBSquareCollisionObject* other, SweepCollision2D & response)
{
	float cosa = cosf(-GetSceneObject()->GetRotationRad());
	float sina = sinf(-GetSceneObject()->GetRotationRad());

	float cosb = 1;
	float sinb = 0;

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
	float penetration = std::numeric_limits<float>::infinity();
	bool isNegative = false;

	float minLeaveTime = std::numeric_limits<float>::infinity();
	float maxEnterTime = -std::numeric_limits<float>::infinity();

	Vec2D rV = GetPhysicsObject()->GetVelocity() - other->GetPhysicsObject()->GetVelocity();

	for (int axes_index = 0; axes_index < 4; axes_index++)
	{
		float mina = std::numeric_limits<float>::infinity();
		float maxa = -std::numeric_limits<float>::infinity();

		float minb = std::numeric_limits<float>::infinity();
		float maxb = -std::numeric_limits<float>::infinity();

		for (int point_index = 0; point_index < 4; point_index++)
		{
			float currenta = derivedPoints[point_index].dot(axis[axes_index]);
			float currentb = otherPoints[point_index].dot(axis[axes_index]);

			mina = min(currenta, mina);
			maxa = max(currenta, maxa);

			minb = min(currentb, minb);
			maxb = max(currentb, maxb);

		}

		float V = rV.dot(axis[axes_index]);

		float currentLeaveTime = 0;
		float currentEnterTime = 0;

		currentLeaveTime = ((maxb - mina) / V);
		currentEnterTime = ((minb - maxa) / V);

		if (currentEnterTime > currentLeaveTime)
		{
			float temp = currentEnterTime;
			currentEnterTime = currentLeaveTime;
			currentLeaveTime = temp;
		}

		minLeaveTime = min(minLeaveTime, currentLeaveTime);
		maxEnterTime = max(maxEnterTime, currentEnterTime);

		if (maxEnterTime > minLeaveTime)return false;
		float overlap = min(maxa + V, maxb) - max(mina + V, minb);

		if (overlap < penetration)
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

	float normalTime = maxEnterTime / GetPhysicsObject()->GetCurrentDeltaTime();

	if (maxEnterTime < minLeaveTime && normalTime <= 1 && normalTime >= 0)
	{
		response.Collision2D.collided = true;
		response.Collision2D.objectBounds[0] = this;
		response.Collision2D.objectBounds[1] = other;
		response.Collision2D.collidedObjects[0] = GetPhysicsObject();
		response.Collision2D.collidedObjects[1] = other->GetPhysicsObject();

		response.timeHit = normalTime;

		return true;
	}
	else return false;
}

Vec2D SatCollision2DObject::ClosesPointOnSegment(const Vec2D& point, const Vec2D& minp, const Vec2D& maxp)
{
	Vec2D newPoint;

	if (point.x > minp.x && point.x < maxp.x)
	{
		newPoint.x = point.x;
	}
	else if (point.x <= minp.x)
	{
		newPoint.x = minp.x;
	}
	else
	{
		newPoint.x = maxp.x;
	}

	if (point.y > minp.y && point.y < maxp.y)
	{
		newPoint.y = point.y;
	}
	else if (point.y <= minp.y)
	{
		newPoint.y = minp.y;
	}
	else
	{
		newPoint.y = maxp.y;
	}


	return newPoint;
}

typedef std::tuple<float, const Vec2D*,bool> pp;
bool compare(const pp&i,const pp&j)
{
	return std::get<0>(i) < std::get<0>(j);
}

int SatCollision2DObject::FindCollisionPoints(CollisionPoint CollisionPointA[2], const Vector2D & normal, const Vector2D otherVertecies[4])
{
	
	float lowestPointA = std::numeric_limits<float>::infinity();
	float lowestPointB = std::numeric_limits<float>::infinity();
	float highestPointA = -std::numeric_limits<float>::infinity();
	float highestPointB = -std::numeric_limits<float>::infinity();

	int lowestIndexA = -1;
	int lowestIndexB = -1;
	int highestIndexA = -1;
	int highestIndexB = -1;

	Vec2D tangent = normal.crossRight(1);

	for (int i = 0; i < 4; i++)
	{
		float projectionA = otherVertecies[i].dot(normal);
		float projectionB = derivedPoints[i].dot(normal);

		if (projectionA < lowestPointB && projectionA == lowestPointA)
		{
			lowestPointB = projectionA;
			lowestIndexB = i;
		}
		else if ( projectionA < lowestPointB && projectionA > lowestPointA)
		{
			lowestPointB = projectionA;
			lowestIndexB = i;
		}
		else if (projectionA < lowestPointA)
		{
			lowestPointB = lowestPointA;
			lowestPointA = projectionA;

			lowestIndexB = lowestIndexA;
			lowestIndexA = i;
		}

		if (projectionB > highestPointB && projectionB == highestPointA)
		{
			highestPointB = projectionB;
			highestIndexB = i;
		}
		else if (projectionB > highestPointB && projectionB < highestPointA)
		{
			highestPointB = projectionB;
			highestIndexB = i;
		}
		else if (projectionB > highestPointA)
		{
			highestPointB = highestPointA;
			highestPointA = projectionB;

			highestIndexB = highestIndexA;
			highestIndexA = i;
		}
	}

	const Vec2D& A0 = otherVertecies[lowestIndexB];
	const Vec2D& A1 = otherVertecies[lowestIndexA];
	const Vec2D& B0 = derivedPoints[highestIndexB];
	const Vec2D& B1 = derivedPoints[highestIndexA];

	pp data[4] = {
		pp(A0.dot(tangent),&A0,true),
		pp(A1.dot(tangent),&A1,true),
		pp(B0.dot(tangent),&B0,false),
		pp(B1.dot(tangent),&B1,false) 
	};												

	std::sort(data, data + 4, compare);

	pp Result1 = data[1];
	pp Result2 = data[2];

	Vec2D B3;
	Vec2D B4;
	Vec2D A3;
	Vec2D A4;

	if (std::get<2>(Result1) && std::get<2>(Result2))
	{
		A3 = *std::get<1>(Result1);
		A4 = *std::get<1>(Result2);

		B3 = ClosesPointOnSegment(*std::get<1>(Result1), B1, B0);
		B4 = ClosesPointOnSegment(*std::get<1>(Result2), B1, B0);
	}
	else if (std::get<2>(Result1) && std::get<2>(Result2) == false)
	{
		A3 = *std::get<1>(Result1);
		B3 = *std::get<1>(Result2);

		B4 = ClosesPointOnSegment(*std::get<1>(Result1), B1, B0);
		A4 = ClosesPointOnSegment(*std::get<1>(Result2), A1, A0);
	}
	else if (std::get<2>(Result1) == false && std::get<2>(Result2))
	{
		B3 = *std::get<1>(Result1);
		A3 = *std::get<1>(Result2);

		A4 = ClosesPointOnSegment(*std::get<1>(Result1), A1, A0);
		B4 = ClosesPointOnSegment(*std::get<1>(Result2), B1, B0);
	}
	else
	{
		B3 = *std::get<1>(Result1);
		B4 = *std::get<1>(Result2);

		A3 = ClosesPointOnSegment(*std::get<1>(Result1), A1, A0);
		A4 = ClosesPointOnSegment(*std::get<1>(Result2), A1, A0);
	}

	int numCollisionPoints = 0;

	static const float elipson = 0.1f;

	if (lowestPointA < (highestPointB + elipson))
	{
		CollisionPointA[numCollisionPoints++].pos = A3;
	}
	if (lowestPointA < (highestPointB + elipson))
	{
		CollisionPointA[numCollisionPoints++].pos = A4;
	}
	
	return numCollisionPoints;
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

void SatCollision2DObject::SetAsCircle(float radius)
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
		float sinv = sinf(GetSceneObject()->GetRotationRad());
		float cosv = cosf(GetSceneObject()->GetRotationRad());

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
		float sinv = sinf(GetSceneObject()->GetRotationRad());
		float cosv = cosf(GetSceneObject()->GetRotationRad());

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

		float verts[12]
		{
			derivedPoints[0].x,derivedPoints[0].y,0,
			derivedPoints[1].x,derivedPoints[1].y,0,
			derivedPoints[2].x,derivedPoints[2].y,0,
			derivedPoints[3].x,derivedPoints[3].y,0,
		};

		UpdateDebugObject(verts, 4);
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

bool SatCollision2DObject::CollidesWith(CollisionObject2DBase * other, Collision2D * response)
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

bool SatCollision2DObject::SweepCollidesWith(CollisionObject2DBase * other, Vector2D newPosition, SweepCollision2D & response)
{
	switch (other->GetCollisionType())
	{
	case SQUARE_COLLISION:
	{
		return SweepTestAgainstOtherAABBSquare((AABBSquareCollisionObject*)other, response);
	}
	break;
	case SAT_2D_COLLISION:
	{
		SatCollision2DObject* satOther = (SatCollision2DObject*)other;
		switch (satOther->polygonType)
		{
		case SATPT_Triangle:
			return SweepTestAgainstOtherTriagnle(satOther, response);
		case SATPT_Square:
			return SweepTestAgainstOtherSquare(satOther, response);
		case SATPT_Circle:
			return SweepTestAgainstOtherCircle(satOther, response);
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

bool SatCollision2DObject::FastSweepCollidesWith(Vector2D newPosition)
{
	return true;
}
