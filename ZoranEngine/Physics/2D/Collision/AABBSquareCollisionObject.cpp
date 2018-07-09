#include "stdafx.h"
#include "AABBSquareCollisionObject.h"
#include <Core/2D/SceneObject2D.h>
#include <Physics/PhysicsEngine.h>
#include <Physics/2D/Collision/CollisionBucket2DBase.h>

AABBSquareCollisionObject::AABBSquareCollisionObject(Vector2D min, Vector2D max, SceneObject2D* object, CollisionDynamics dynamics,bool isRoot) : CollisionObject2DBase(object, dynamics, SQUARE_COLLISION)
{
	this->minPos = min;
	this->maxPos = max;

	this->size = maxPos - minPos;
	this->scaledSize = this->size;

	if(isRoot)sweepCollisionSquare = new AABBSquareCollisionObject(Vec2D(), Vec2D(), object, CD_Static,false);
}

AABBSquareCollisionObject::~AABBSquareCollisionObject()
{
	delete sweepCollisionSquare;
}

void AABBSquareCollisionObject::SetBoundsBySceneObject()
{
	Vector2D pos = GetScenePos();
	Vector2D scale = GetSceneObject()->GetScale().getAbs();

	scaledSize = size * scale;

	minPos = pos - (scaledSize / 2);
	maxPos = pos + (scaledSize / 2);
}

bool AABBSquareCollisionObject::CollidesWith(CollisionObject2DBase* other, CollisionResponse2D& response)
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

Vector2D AABBSquareCollisionObject::GetClosestPointTo(Vector2D pos)
{
	Vector2D point;

	point.x = max(this->minPos.x, min(pos.x, this->maxPos.x));
	point.y = max(this->minPos.y, min(pos.y, this->maxPos.y));

	return point;
}

bool AABBSquareCollisionObject::SweepCollidesWith(CollisionObject2DBase * other, Vector2D newPosition, SweepCollisionResponse2D & response)
{
	Vec2D origin = GetScenePos();
	Vec2D deltaPos = newPosition - origin;
	Vec2D size = GetSize();

	Vec2D InvEntry;
	Vec2D InvExit;

	Vec2D otherPos = other->GetScenePos();
	Vec2D otherSize = other->GetSize();

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
		entry.x = -std::numeric_limits<double>::infinity();
		exit.x = -std::numeric_limits<double>::infinity();
	}
	else
	{
		entry.x = InvEntry.x / deltaPos.x;
		exit.x = InvExit.x / deltaPos.x;
	}
	if (deltaPos.y == 0)
	{
		entry.y = -std::numeric_limits<double>::infinity();
		exit.y = -std::numeric_limits<double>::infinity();
	}
	else
	{
		entry.y = InvEntry.y / deltaPos.y;
		exit.y = InvExit.y / deltaPos.y;
	}

	double entryTime = max(entry.x, entry.y);
	double exitTime = max(exit.x, exit.y);

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


	response.timeHit = entryTime;

	response.CollisionResponse2D.collided = true;
	response.CollisionResponse2D.collidedObjects[0] = GetPhysicsObject();
	response.CollisionResponse2D.collidedObjects[1] = other->GetPhysicsObject();
	response.CollisionResponse2D.objectBounds[0] = this;
	response.CollisionResponse2D.objectBounds[1] = other;
	response.CollisionResponse2D.normal = normal;

	return true;

}

bool AABBSquareCollisionObject::FastSweepCollidesWith(Vector2D newPosition)
{
	Vec2D startingPosition = GetSceneObject()->GetPosition();
	Vec2D size = GetSceneObject()->GetCollision2D()->GetSize() / 2.0;

	double minPositionx = min(startingPosition.x, newPosition.x);
	double maxPositionx = max(startingPosition.x, newPosition.x);

	double minPositiony = min(startingPosition.y, newPosition.y);
	double maxPositiony = max(startingPosition.y, newPosition.y);

	sweepCollisionSquare->minPos.x = minPositionx - size.x;
	sweepCollisionSquare->maxPos.x = maxPositionx + size.x;

	sweepCollisionSquare->minPos.y = minPositiony - size.y;
	sweepCollisionSquare->maxPos.y = maxPositiony + size.y;

	CollisionResponse2D unused;
	return pEngine->GetCollisionBucketRootFor2D()->CheckObjectAgainstStaic(sweepCollisionSquare, unused);
}


Vector2D AABBSquareCollisionObject::GetSize()
{
	return Vector2D(maxPos - minPos);
}
