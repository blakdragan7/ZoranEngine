#include "stdafx.h"
#include "MathLib.h"
#include <Math/Vector2.h>

int MathLib::ClipRangeToLine(Vector2D outPoints[2], Vector2D inPoints[2], const Vector2D & lineNormal, double offset)
{
	int numPoints = 0;

	// Calculate the distance of end points to the line
	double distance0 = lineNormal.dot(inPoints[0]) - offset;
	double distance1 = lineNormal.dot(inPoints[1]) - offset;

	// If the points are behind the plane
	if (distance0 <= 0.0f) outPoints[numPoints++] = inPoints[0];
	if (distance1 <= 0.0f) outPoints[numPoints++] = inPoints[1];

	// If the points are on different sides of the plane
	if (distance0 * distance1 < 0.0f)
	{
		// Find intersection point of edge and plane
		double interp = distance0 / (distance0 - distance1);
		outPoints[numPoints] = inPoints[0] + interp * (inPoints[1] - inPoints[0]);
		if (distance0 > 0.0f)
		{
			outPoints[numPoints].fp = vIn[0].fp;
			outPoints[numPoints].fp.e.inEdge1 = clipEdge;
			outPoints[numPoints].fp.e.inEdge2 = NO_EDGE;
		}
		else
		{
			outPoints[numPoints].fp = vIn[1].fp;
			outPoints[numPoints].fp.e.outEdge1 = clipEdge;
			outPoints[numPoints].fp.e.outEdge2 = NO_EDGE;
		}
		++numPoints;
	}

	return numPoints;
}
