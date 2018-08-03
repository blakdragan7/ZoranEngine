#include "stdafx.h"
#include <string>
#include "b2DCollision2DObject.h"

#include <Math/MathLib.h>
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/2D/PhysicsObject2DBase.h>

#include <Physics/2D/Collision/CircleCollision2DObject.h>

#define BOTTOM_LEFT 0
#define TOP_LEFT 1
#define BOTTOM_RIGHT 2
#define TOP_RIGHT 3

b2DCollision2DObject::b2DCollision2DObject(SceneObject2D *object) : CollisionObject2DBase(object,CD_Dynamic, b2D_2D_COLLISION)
{
}

b2DCollision2DObject::~b2DCollision2DObject()
{
}

void b2DCollision2DObject::SetBoundsBySceneObject()
{
	// unused
}

Vector2D b2DCollision2DObject::GetSize()
{
	return GetSceneObject()->GetScale();
}

bool b2DCollision2DObject::CollidesWithNoCollision(CollisionObject2DBase * other)
{
	return false;
}

Collision2D* b2DCollision2DObject::CollidesWith(CollisionObject2DBase * other)
{
	if (other->GetCollisionType() == CIRCLE_COLLISION)
	{
		return other->CollidesWith(this);
	}

	SceneObject2D* objectA = GetSceneObject();
	SceneObject2D* objectB = other->GetSceneObject();

	Vector2D halfA = 0.5f * objectA->GetSize();
	Vector2D halfB = 0.5f * objectB->GetSize();

	Vector2D posA = objectA->GetPosition();
	Vector2D posB = objectB->GetPosition();

	Matrix22 RotA(objectA->GetRotation()), RotB(objectB->GetRotation());

	Matrix22 RotAT = RotA.GetTranspose();
	Matrix22 RotBT = RotB.GetTranspose();

	Vector2D a1 = RotA.cols[0];
	Vector2D a2 = RotA.cols[1];
	Vector2D b1 = RotB.cols[0];
	Vector2D b2 = RotB.cols[1];

	Vector2D deltaPos = posB - posA;
	Vector2D dA = RotAT * deltaPos;
	Vector2D dB = RotBT * deltaPos;

	Matrix22 C = RotAT * RotB;
	Matrix22 absC = C.GetAbs();
	Matrix22 absCT = absC.GetTranspose();

	// Box A faces
	Vector2D faceA = dA.getAbs() - halfA - absC * halfB;
	if (faceA.x > 0.0f || faceA.y > 0.0f)
		return false;

	// Box B faces
	Vector2D faceB = dB.getAbs() - absCT * halfA - halfB;
	if (faceB.x > 0.0f || faceB.y > 0.0f)
		return false;

	// Find best axis
	Axis axis;
	float separation;
	Vector2D normal;

	// Box A faces
	axis = FACE_A_X;
	separation = faceA.x;
	normal = dA.x > 0.0f ? RotA.cols[0] : -RotA.cols[0];

	static const float relativeTol = 0.95f;
	static const float absoluteTol = 0.01f;

	if (faceA.y > relativeTol * separation + absoluteTol * halfA.y)
	{
		axis = FACE_A_Y;
		separation = faceA.y;
		normal = dA.y > 0.0f ? RotA.cols[1] : -RotA.cols[1];
	}

	// Box B faces
	if (faceB.x > relativeTol * separation + absoluteTol * halfB.x)
	{
		axis = FACE_B_X;
		separation = faceB.x;
		normal = dB.x > 0.0f ? RotB.cols[0] : -RotB.cols[0];
	}

	if (faceB.y > relativeTol * separation + absoluteTol * halfB.y)
	{
		axis = FACE_B_Y;
		separation = faceB.y;
		normal = dB.y > 0.0f ? RotB.cols[1] : -RotB.cols[1];
	}

	// Setup clipping plane data based on the separating axis
	Vector2D frontNormal, sideNormal;
	ClipedVertex incidentEdge[2];
	float front, negSide, posSide;
	char negEdge, posEdge;

	// Compute the clipping lines and the line segment to be clipped.
	switch (axis)
	{
	case FACE_A_X:
	{
		frontNormal = normal;
		front = posA.dot(frontNormal) + halfA.x;
		sideNormal = RotA.cols[1];
		float side = posA.dot(sideNormal);
		negSide = -side + halfA.y;
		posSide = side + halfA.y;
		negEdge = EDGE3;
		posEdge = EDGE1;
		MathLib::ComputeIncidentEdge(incidentEdge, halfB, posB, RotB, frontNormal);
	}
	break;

	case FACE_A_Y:
	{
		frontNormal = normal;
		front = posA.dot(frontNormal) + halfA.y;
		sideNormal = RotA.cols[0];
		float side = posA.dot(sideNormal);
		negSide = -side + halfA.x;
		posSide = side + halfA.x;
		negEdge = EDGE2;
		posEdge = EDGE4;
		MathLib::ComputeIncidentEdge(incidentEdge, halfB, posB, RotB, frontNormal);
	}
	break;

	case FACE_B_X:
	{
		frontNormal = -normal;
		front = posB.dot(frontNormal) + halfB.x;
		sideNormal = RotB.cols[1];
		float side = posB.dot(sideNormal);
		negSide = -side + halfB.y;
		posSide = side + halfB.y;
		negEdge = EDGE3;
		posEdge = EDGE1;
		MathLib::ComputeIncidentEdge(incidentEdge, halfA, posA, RotA, frontNormal);
	}
	break;

	case FACE_B_Y:
	{
		frontNormal = -normal;
		front = posB.dot(frontNormal) + halfB.y;
		sideNormal = RotB.cols[0];
		float side = posB.dot(sideNormal);
		negSide = -side + halfB.x;
		posSide = side + halfB.x;
		negEdge = EDGE2;
		posEdge = EDGE4;
		MathLib::ComputeIncidentEdge(incidentEdge, halfA, posA, RotA, frontNormal);
	}
	break;
	}

	// clip other face with 5 box planes (1 face plane, 4 edge planes)

	ClipedVertex clipPoints1[2];
	ClipedVertex clipPoints2[2];
	int np;

	// Clip to box side 1
	np = MathLib::ClipRangeToLine(clipPoints1, incidentEdge, -sideNormal, negSide, negEdge);

	if (np < 2)
		return false;

	// Clip to negative box side 1
	np = MathLib::ClipRangeToLine(clipPoints2, clipPoints1, sideNormal, posSide, posEdge);

	if (np < 2)
		return false;

	// Now clipPoints2 contains the clipping points.
	// Due to roundoff, it is possible that clipping removes all points.

	CollisionPoint points[2];

	unsigned numPoints = 0;
	for (int i = 0; i < 2; ++i)
	{
		float separation = frontNormal.dot(clipPoints2[i].vertex) - front;

		if (separation <= 0)
		{
			CollisionPoint point;

			point.separation = separation;
			point.normal = normal;
			// slide contact point onto reference face (easy to cull)
			point.pos = clipPoints2[i].vertex - separation * frontNormal;
			point.edges = clipPoints2[i].edge;
			if (axis == FACE_B_X || axis == FACE_B_Y)
				point.edges.Flip();

			points[numPoints++] = point;
		}
	}
	if (numPoints > 0)
	{
		Collision2D* collision = new Collision2D();

		collision->collided = true;
		collision->objects[0] = objectA;
		collision->objects[1] = objectB;
		collision->collidedObjects[0] = this->GetPhysicsObject();
		collision->collidedObjects[1] = other->GetPhysicsObject();
		collision->objectBounds[0] = this;
		collision->objectBounds[1] = other;
		for (unsigned i = 0; i < numPoints; i++)
		{
			collision->AddCollisionPoint(points[i]);
		}
		collision->friction = sqrt(objectA->GetPhysics()->GetFriction() * objectB->GetPhysics()->GetFriction());

		return collision;
	}
	return 0;
}

Vector2D b2DCollision2DObject::GetClosestPointTo(Vector2D pos)
{
	return pos;
}

bool b2DCollision2DObject::SweepCollidesWith(CollisionObject2DBase * other, Vector2D newPosition, SweepCollision2D & response)
{
	return false;
}

bool b2DCollision2DObject::FastSweepCollidesWith(Vector2D newPosition)
{
	return true;
}
