#include "stdafx.h"
#include "QuadTreeCollision.h"
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/3D/Collision/AABBoxCollisionObject.h>
#include <Physics/3D/Collision/SphereCollisionObject.h>
#include "Core/SceneObject.h"
#include <Math/Vector2.h>

QuadTreeCollision::QuadTreeCollision(Vector2D min, Vector2D max, SceneObject2D* object) : CollisionObject2DBase(object,CD_Static, QUADTREE_COLLISION)
{
	this->minPos = min;
	this->maxPos = max;

	this->size = maxPos - minPos;
	this->scaledSize = this->size;
}


QuadTreeCollision::~QuadTreeCollision()
{
}

void QuadTreeCollision::SetBoundsBySceneObject()
{
	Vector2D pos = GetScenePos();
	Vector2D scale = GetSceneObject()->GetScale().getAbs();

	scaledSize = size * scale;

	minPos = pos - (scaledSize / 2);
	maxPos = pos + (scaledSize / 2);
}

bool QuadTreeCollision::CollidesWith(Vector3D pos)
{
	return (pos.x >= minPos.x && pos.x <= maxPos.x) && \
		(pos.y >= minPos.y && pos.y <= maxPos.y);
}

Collision2D* QuadTreeCollision::CollidesWith(CollisionObject2DBase * other)
{
	return 0;
}

Vector2D QuadTreeCollision::GetClosestPointTo(Vector2D pos)
{
	Vector2D point;

	point.x = max(this->minPos.x, min(pos.x, this->maxPos.x));
	point.y = max(this->minPos.y, min(pos.y, this->maxPos.y));

	return point;
}

Vector2D QuadTreeCollision::GetSize()
{
	return Vector2D((maxPos - minPos));
}

bool QuadTreeCollision::CollidesWithNoCollision(CollisionObject2DBase * other)
{
	bool collides = false;

	switch (other->GetCollisionType())
	{
	case AABBOX_COLLISION:
	{
		AABBoxCollisionObject* otherBox = (AABBoxCollisionObject*)other;
		collides = CollidesWith(otherBox->GetMinPos()) && CollidesWith(otherBox->GetMaxPos());
	}
	break;
	case SPHERE_COLLISION:
	{
		SphereCollisionObject* sphere = (SphereCollisionObject*)other;
		float r = sphere->GetRadius();
		Vec3D pos = sphere->GetScenePos();

		collides = CollidesWith(pos - r) && CollidesWith(pos + r);
	}
	break;
	}

	return (Collision2D*)collides;
}
