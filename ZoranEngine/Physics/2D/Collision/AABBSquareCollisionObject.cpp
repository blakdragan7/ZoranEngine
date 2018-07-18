#include "stdafx.h"
#include "AABBSquareCollisionObject.h"
#include <Core/2D/SceneObject2D.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/2D/Collision/CollisionBucket2DBase.h>
#include <Physics/2D/PhysicsObject2DBase.h>

void AABBSquareCollisionObject::FillCollisionPoints(CollisionPoint points[2], Vec2D otherMin, Vec2D otherMax)
{
	points[0].pos.x = max(minPos.x, otherMin.x);
	points[0].pos.y = max(minPos.y, otherMin.y);

	points[1].pos.x = min(minPos.x, otherMin.x);
	points[1].pos.y = min(maxPos.y, otherMax.y);
}

AABBSquareCollisionObject::AABBSquareCollisionObject(Vector2D min, Vector2D max, SceneObject2D* object, CollisionDynamics dynamics,bool isRoot) : CollisionObject2DBase(object, dynamics, SQUARE_COLLISION)
{
	this->minPos = min;
	this->maxPos = max;

	this->size = maxPos - minPos;

	if(isRoot)sweepCollisionSquare = new AABBSquareCollisionObject(Vec2D(), Vec2D(), object, CD_Static,false);
}

AABBSquareCollisionObject::~AABBSquareCollisionObject()
{
	delete sweepCollisionSquare;
}

void AABBSquareCollisionObject::SetBoundsBySceneObject()
{
	Vector2D pos = GetScenePos();
	Vector2D size = GetSceneObject()->GetSize();

	minPos = pos - (size / 2);
	maxPos = pos + (size / 2);
}

bool AABBSquareCollisionObject::CollidesWith(CollisionObject2DBase* other, Collision2D* response)
{
	Vec2D otherMin;
	Vec2D otherMax;

	if (other->GetCollisionType() == SQUARE_COLLISION)
	{
		AABBSquareCollisionObject* squareOther = (AABBSquareCollisionObject*)other;
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

	static const Vector2D faces[4] =
	{
		Vector2D(-1,  0), // 'left' face normal (-x direction)
		Vector2D(1,  0), // 'right' face normal (+x direction)
		Vector2D(0, -1), // 'bottom' face normal (-y direction)
		Vector2D(0,  1), // 'top' face normal (+y direction)
	};

	float distances[4] =
	{
		(otherMax.x - minPos.x), // distance of otherBox to face on 'left' side.
		(maxPos.x - otherMin.x), // distance of otherBox to face on 'right' side.
		(otherMax.y - minPos.y), // distance of otherBox to face on 'bottom' side.
		(maxPos.y - otherMin.y), // distance of otherBox to face on 'top' side.
	};

	float penetration = 0;
	Vec2D normal;
	int faceIndex = -1;

	CollisionPoint contactPointsA[2];

	for (int i = 0; i < 4; i++)
	{
		// box does not intersect face. So boxes don't intersect at all.
		if (distances[i] < 0.0f)
		{
			response->collided = false;
			return false;
		}
		// face of least intersection depth. That's our candidate.
		if ((i == 0) || (distances[i] < penetration))
		{
			faceIndex = i;

			penetration = distances[i];

			FillCollisionPoints(contactPointsA, otherMin, otherMax);

			contactPointsA[0].normal = faces[i];
			/*contactPointsA[0].penetrationDepth = distances[i];
			contactPointsA[0].penetration = -contactPointsA[0].normal * distances[i];*/

			
			contactPointsA[1].normal = contactPointsA[0].normal;
			/*contactPointsA[1].penetrationDepth = contactPointsA[0].penetrationDepth;
			contactPointsA[1].penetration = contactPointsA[0].penetration;*/
		}
	}

	response->collidedObjects[0] = GetPhysicsObject();
	response->collidedObjects[1] = other->GetPhysicsObject();
	response->objectBounds[0] = this;
	response->objectBounds[1] = other;
	response->collided = true;
	response->objects[0] = GetSceneObject();
	response->objects[1] = other->GetSceneObject();
	response->AddCollisionPoint(contactPointsA[0]);
	response->AddCollisionPoint(contactPointsA[1]);

	if (GetPhysicsObject())
		response->velocitySnapshot[0] = GetPhysicsObject()->GetVelocity();
	if(other->GetPhysicsObject())
		response->velocitySnapshot[1] = other->GetPhysicsObject()->GetVelocity();

	return true;
}

Vector2D AABBSquareCollisionObject::GetClosestPointTo(Vector2D pos)
{
	Vector2D point;

	point.x = max(this->minPos.x, min(pos.x, this->maxPos.x));
	point.y = max(this->minPos.y, min(pos.y, this->maxPos.y));

	return point;
}

bool AABBSquareCollisionObject::SweepCollidesWith(CollisionObject2DBase * other, Vector2D newPosition, SweepCollision2D & response)
{
	Vec2D origin = GetScenePos();
	Vec2D deltaPos = newPosition - origin;
	Vec2D size = GetSize();

	Vec2D InvEntry;
	Vec2D InvExit;

	Vec2D otherPos = other->GetScenePos();
	Vec2D otherSize = other->GetSize();

	Vec2D otherMin = otherPos - (otherSize / 2.0);
	Vec2D otherMax = otherPos + (otherSize / 2.0);

	// determine closest and farthest axis

	if (deltaPos.x > 0)
	{
		InvEntry.x = otherPos.x - (origin.x + size.w);
		InvExit.x = (otherPos.x + otherSize.w) - origin.x;
	}
	else
	{
		InvEntry.x = (otherPos.x + otherSize.w) - origin.x;
		InvExit.x = otherPos.x - (origin.x + size.w);
	}
	if (deltaPos.y > 0.0f)
	{
		InvEntry.y = otherPos.y - (origin.y + size.h);
		InvExit.y = (otherPos.y + otherSize.h) - origin.y;
	}
	else
	{
		InvEntry.y = (otherPos.y + otherSize.h) - origin.y;
		InvExit.y = otherPos.y - (origin.y + size.h);
	}

	Vec2D entry;
	Vec2D exit;

	if (deltaPos.x == 0)
	{
		entry.x = -std::numeric_limits<float>::infinity();
		exit.x = -std::numeric_limits<float>::infinity();
	}
	else
	{
		entry.x = InvEntry.x / deltaPos.x;
		exit.x = InvExit.x / deltaPos.x;
	}
	if (deltaPos.y == 0)
	{
		entry.y = -std::numeric_limits<float>::infinity();
		exit.y = -std::numeric_limits<float>::infinity();
	}
	else
	{
		entry.y = InvEntry.y / deltaPos.y;
		exit.y = InvExit.y / deltaPos.y;
	}

	float entryTime = max(entry.x, entry.y);
	float exitTime = max(exit.x, exit.y);

	if (entryTime > exitTime || (entry.x < 0.0 && entry.y < 0) || (entry.x > 1.0  && entry.y > 1.0))
	{
		return false;
	}

	Vec2D normal;

	if (entry.x > entry.y)
	{
		if (entry.x < 0) normal = Vec2D(-1, 0);
		else normal = Vec2D(1, 0);
	}
	{
		if (entry.y < 0) normal = Vec2D(0, -1);
		else normal = Vec2D(0, 1);
	}

	CollisionPoint points[2];

	FillCollisionPoints(points, otherMin, otherMax);

	points[0].normal = normal;
	points[1].normal = normal;

	response.timeHit = entryTime;

	response.Collision2D.collided = true;
	response.Collision2D.collidedObjects[0] = GetPhysicsObject();
	response.Collision2D.collidedObjects[1] = other->GetPhysicsObject();
	response.Collision2D.objectBounds[0] = this;
	response.Collision2D.objectBounds[1] = other;

	response.Collision2D.AddCollisionPoint(points[0]);
	response.Collision2D.AddCollisionPoint(points[1]);
	
	if (GetPhysicsObject())
		response.Collision2D.velocitySnapshot[0] = GetPhysicsObject()->GetVelocity();
	if (other->GetPhysicsObject())
		response.Collision2D.velocitySnapshot[1] = other->GetPhysicsObject()->GetVelocity();

	return true;

}

bool AABBSquareCollisionObject::FastSweepCollidesWith(Vector2D newPosition)
{
	Vec2D startingPosition = GetSceneObject()->GetPosition();
	Vec2D size = GetSceneObject()->GetCollision2D()->GetSize() / 2.0;

	float minPositionx = min(startingPosition.x, newPosition.x);
	float maxPositionx = max(startingPosition.x, newPosition.x);

	float minPositiony = min(startingPosition.y, newPosition.y);
	float maxPositiony = max(startingPosition.y, newPosition.y);

	sweepCollisionSquare->minPos.x = minPositionx - size.x;
	sweepCollisionSquare->maxPos.x = maxPositionx + size.x;

	sweepCollisionSquare->minPos.y = minPositiony - size.y;
	sweepCollisionSquare->maxPos.y = maxPositiony + size.y;

	static Collision2D *unused = new Collision2D();
	return pEngine->GetCollisionBucketRootFor2D()->CheckObjectAgainstStaic(sweepCollisionSquare, unused);
}


Vector2D AABBSquareCollisionObject::GetSize()
{
	return Vector2D(maxPos - minPos);
}

bool AABBSquareCollisionObject::CollidesWithNoCollision(CollisionObject2DBase * other)
{
	Vec2D otherMin;
	Vec2D otherMax;

	if (other->GetCollisionType() == SQUARE_COLLISION)
	{
		AABBSquareCollisionObject* squareOther = (AABBSquareCollisionObject*)other;
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

	static const Vector2D faces[4] =
	{
		Vector2D(-1,  0), // 'left' face normal (-x direction)
		Vector2D(1,  0), // 'right' face normal (+x direction)
		Vector2D(0, -1), // 'bottom' face normal (-y direction)
		Vector2D(0,  1), // 'top' face normal (+y direction)
	};

	float distances[4] =
	{
		(otherMax.x - minPos.x), // distance of otherBox to face on 'left' side.
		(maxPos.x - otherMin.x), // distance of otherBox to face on 'right' side.
		(otherMax.y - minPos.y), // distance of otherBox to face on 'bottom' side.
		(maxPos.y - otherMin.y), // distance of otherBox to face on 'top' side.
	};

	CollisionPoint contactPointsA[2];

	for (int i = 0; i < 4; i++)
	{
		// box does not intersect face. So boxes don't intersect at all.
		if (distances[i] < 0.0f)
		{
			return false;
		}
	}

	return true;
}
