#include "stdafx.h"
#include "MathLib.h"
#include <Math/Vector2.h>
#include <Math/Matrix22.hpp>
#include <Physics/2D/Collision/CollisionObject2DBase.h>

int MathLib::ClipRangeToLine(ClipedVertex outPoints[2], ClipedVertex inPoints[2], const Vector2D & lineNormal, float offset, char clipEdge)
{
	int numPoints = 0;

	// Calculate the distance of end points to the line
	float distance0 = lineNormal.dot(inPoints[0].vertex) - offset;
	float distance1 = lineNormal.dot(inPoints[1].vertex) - offset;

	// If the points are behind the plane
	if (distance0 <= 0.0f) outPoints[numPoints++] = inPoints[0];
	if (distance1 <= 0.0f) outPoints[numPoints++] = inPoints[1];

	// If the points are on different sides of the plane
	if (distance0 * distance1 < 0.0f)
	{
		// Find intersection point of edge and plane
		float interp = distance0 / (distance0 - distance1);
		outPoints[numPoints].vertex = inPoints[0].vertex + interp * (inPoints[1].vertex - inPoints[0].vertex);
		//outPoints[numPoints].vertex.x = roundf(outPoints[numPoints].vertex.x);
		if (distance0 > 0.0f)
		{
			outPoints[numPoints].edge = inPoints[0].edge;
			outPoints[numPoints].edge.e.inEdge1 = clipEdge;
			outPoints[numPoints].edge.e.inEdge2 = NO_EDGE;
		}
		else
		{
			outPoints[numPoints].edge = inPoints[1].edge;
			outPoints[numPoints].edge.e.outEdge1 = clipEdge;
			outPoints[numPoints].edge.e.outEdge2 = NO_EDGE;
		}
		++numPoints;
	}

	return numPoints;
}

void MathLib::ComputeIncidentEdge(ClipedVertex c[2], const Vector2D & h, const Vector2D & pos, const Matrix22 & Rot, const Vector2D & normal)
{
	// The normal is from the reference box. Convert it
	// to the incident boxe's frame and flip sign.
	Matrix22 RotT = Rot.GetTranspose();
	Vector2D n = -(RotT * normal);
	Vector2D nAbs = n.getAbs();

	if (nAbs.x > nAbs.y)
	{
		if (signum(n.x) > 0.0f)
		{
			c[0].vertex.Set(h.x, -h.y);
			c[0].edge.e.inEdge2 = EDGE3;
			c[0].edge.e.outEdge2 = EDGE4;

			c[1].vertex.Set(h.x, h.y);
			c[1].edge.e.inEdge2 = EDGE4;
			c[1].edge.e.outEdge2 = EDGE1;
		}
		else
		{
			c[0].vertex.Set(-h.x, h.y);
			c[0].edge.e.inEdge2 = EDGE1;
			c[0].edge.e.outEdge2 = EDGE2;

			c[1].vertex.Set(-h.x, -h.y);
			c[1].edge.e.inEdge2 = EDGE2;
			c[1].edge.e.outEdge2 = EDGE3;
		}
	}
	else
	{
		if (signum(n.y) > 0.0f)
		{
			c[0].vertex.Set(h.x, h.y);
			c[0].edge.e.inEdge2 = EDGE4;
			c[0].edge.e.outEdge2 = EDGE1;

			c[1].vertex.Set(-h.x, h.y);
			c[1].edge.e.inEdge2 = EDGE1;
			c[1].edge.e.outEdge2 = EDGE2;
		}
		else
		{
			c[0].vertex.Set(-h.x, -h.y);
			c[0].edge.e.inEdge2 = EDGE2;
			c[0].edge.e.outEdge2 = EDGE3;

			c[1].vertex.Set(h.x, -h.y);
			c[1].edge.e.inEdge2 = EDGE3;
			c[1].edge.e.outEdge2 = EDGE4;
		}
	}

	c[0].vertex = pos + Rot * c[0].vertex;
	c[1].vertex = pos + Rot * c[1].vertex;
}

float MathLib::Clamp(float value, float minv, float maxv)
{
	return max(minv,min(value,maxv));
}

Vector2D MathLib::Clamp(Vector2D value, Vector2D minv, Vector2D maxv)
{
	value.x = Clamp(value.x, minv.x, maxv.x);
	value.y = Clamp(value.y, minv.y, maxv.y);

	return value;
}
