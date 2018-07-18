#include "stdafx.h"
#include <string>
#include "SatCollision2DObject.h"

#include <Math/MathLib.h>
#include <Core/2D/SceneObject2D.h>
#include <Physics/2D/Collision/AABBSquareCollisionObject.h>
#include <Physics/2D/PhysicsObject2DBase.h>

#define BOTTOM_LEFT 0
#define TOP_LEFT 1
#define BOTTOM_RIGHT 2
#define TOP_RIGHT 3

bool SatCollision2DObject::SquareAgainstOtherSquare(SatCollision2DObject * other, Collision2D * response)
{
	return NewSATAlgorithim(other, response);
}

bool SatCollision2DObject::SquareAgainstOtherTriagnle(SatCollision2DObject * other, Collision2D * response)
{
	return NewSATAlgorithim(other, response);
}

bool SatCollision2DObject::SquareAgainstOtherCircle(SatCollision2DObject * other, Collision2D * response)
{
	return NewSATAlgorithim(other, response);
}

bool SatCollision2DObject::SquareAgainstOtherAABBSquare(AABBSquareCollisionObject * other, Collision2D * response)
{
	// TODO optimize to use specific function

	return NewSATAlgorithim(other, response);
}

bool SatCollision2DObject::SweepSquareAgainstOtherSquare(SatCollision2DObject* other, SweepCollision2D & response)
{
	return false;
}

bool SatCollision2DObject::SweepSquareAgainstOtherTriagnle(SatCollision2DObject* other, SweepCollision2D & response)
{
	return false;
}

bool SatCollision2DObject::SweepSquareAgainstOtherCircle(SatCollision2DObject* other, SweepCollision2D & response)
{
	return false;
}

bool SatCollision2DObject::SweepSquareAgainstOtherAABBSquare(AABBSquareCollisionObject* other, SweepCollision2D & response)
{
	return false;
}

bool SatCollision2DObject::NewSATAlgorithim(CollisionObject2DBase * other, Collision2D * response)
{
	// More effeicient SAT algorithim taking from Box 2ds Light,
	// Previous algorithim is, find two axis and project all verts onto axes, check overlap between projected verts.
	// New Algorithim, transform B into A's space (make B relative to A  including rotation) get distance between A and B and project it onto 
	//	two axis. The axis is chosen based on B's location compared to A. Based on Axes, compute incident edge (the edge the collision happens on) and then use that information
	//  to find cliped vertecies, verts projected to axes, which give collision points. If at any time no cliped vertecis can be 
	//  found then there is no collision or if relative to A b.s positions are both negative there is no collision

	// Setup

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

	const float relativeTol = 0.95f;
	const float absoluteTol = 0.01f;

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
			response->AddCollisionPoint(point);
		}
	}
	if (response->GetNumCollisionPoints() > 0)
	{
		response->collided = true;
		response->objects[0] = objectA;
		response->objects[1] = objectB;
		response->collidedObjects[0] = this->GetPhysicsObject();
		response->collidedObjects[1] = other->GetPhysicsObject();
		response->objectBounds[0] = this;
		response->objectBounds[1] = other;
		response->rotationSnapshots[0] = objectA->GetRotationMatrix();
		response->rotationSnapshots[1] = objectB->GetRotationMatrix();
		response->invRotationSnapshots[0] = objectA->GetInvRotationMatrix();
		response->invRotationSnapshots[1] = objectB->GetInvRotationMatrix();
		response->friction = sqrt(objectA->GetPhysics()->GetFriction() * objectB->GetPhysics()->GetFriction());
		if (GetDynamics() != CD_Static)
			response->velocitySnapshot[0] = response->collidedObjects[0]->GetVelocity();
		if (other->GetDynamics() != CD_Static)
			response->velocitySnapshot[1] = response->collidedObjects[1]->GetVelocity();
	}

	return response->collided;
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
		float sinfv = sinf(GetSceneObject()->GetRotationRad());
		float cosfv = cosf(GetSceneObject()->GetRotationRad());

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

		derivedPoints[0].x = (cosfv * tempPoinnts[0].x) - (sinfv * tempPoinnts[0].y);
		derivedPoints[0].y = (sinfv * tempPoinnts[0].x) + (cosfv * tempPoinnts[0].y);

		derivedPoints[1].x = (cosfv * tempPoinnts[1].x) - (sinfv * tempPoinnts[1].y);
		derivedPoints[1].y = (sinfv * tempPoinnts[1].x) + (cosfv * tempPoinnts[1].y);

		derivedPoints[2].x = (cosfv * tempPoinnts[2].x) - (sinfv * tempPoinnts[2].y);
		derivedPoints[2].y = (sinfv * tempPoinnts[2].x) + (cosfv * tempPoinnts[2].y);

		derivedPoints[0] += pos;
		derivedPoints[1] += pos;
		derivedPoints[2] += pos;

	}
		break;
	case SATPT_Square:
	{
		float sinfv = sinf(GetSceneObject()->GetRotationRad());
		float cosfv = cosf(GetSceneObject()->GetRotationRad());

		Vec2D scale = GetSceneObject()->GetScale();
		Vec2D pos = GetScenePos();

		scaledSize = scale * size;

		derivedPoints[0].x = (cosfv * startingPoints[0].x) - (sinfv * startingPoints[0].y);
		derivedPoints[0].y = (sinfv * startingPoints[0].x) + (cosfv * startingPoints[0].y);

		derivedPoints[1].x = (cosfv * startingPoints[1].x) - (sinfv * startingPoints[1].y);
		derivedPoints[1].y = (sinfv * startingPoints[1].x) + (cosfv * startingPoints[1].y);

		derivedPoints[2].x = (cosfv * startingPoints[2].x) - (sinfv * startingPoints[2].y);
		derivedPoints[2].y = (sinfv * startingPoints[2].x) + (cosfv * startingPoints[2].y);

		derivedPoints[3].x = (cosfv * startingPoints[3].x) - (sinfv * startingPoints[3].y);
		derivedPoints[3].y = (sinfv * startingPoints[3].x) + (cosfv * startingPoints[3].y);

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

bool SatCollision2DObject::CollidesWithNoCollision(CollisionObject2DBase * other)
{
	return false;
}

bool SatCollision2DObject::CollidesWith(CollisionObject2DBase * other, Collision2D * response)
{
	switch (other->GetCollisionType())
	{
	case SQUARE_COLLISION:
	{
		return SquareAgainstOtherAABBSquare((AABBSquareCollisionObject*)other, response);
	}
		break;
	case SAT_2D_COLLISION:
	{
		SatCollision2DObject* satOther = (SatCollision2DObject*)other;
		switch (satOther->polygonType)
		{
		case SATPT_Triangle:
			return SquareAgainstOtherTriagnle(satOther, response);
		case SATPT_Square:
			return SquareAgainstOtherSquare(satOther, response);
		case SATPT_Circle:
			return SquareAgainstOtherCircle(satOther, response);
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
		return SweepSquareAgainstOtherAABBSquare((AABBSquareCollisionObject*)other, response);
	}
	break;
	case SAT_2D_COLLISION:
	{
		SatCollision2DObject* satOther = (SatCollision2DObject*)other;
		switch (satOther->polygonType)
		{
		case SATPT_Triangle:
			return SweepSquareAgainstOtherTriagnle(satOther, response);
		case SATPT_Square:
			return SweepSquareAgainstOtherSquare(satOther, response);
		case SATPT_Circle:
			return SweepSquareAgainstOtherCircle(satOther, response);
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
