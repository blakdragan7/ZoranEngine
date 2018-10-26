#pragma once
#include <Math/Vector3.h>
#include <Math/Vector2.h>

struct Color
{
	float r, g, b, a;
};

struct RectanglePrimitive
{
	Vector3D vertecies[4];
	Vector2D coords[4];
};

struct TrianglePrimitive
{
	Vector3D vertecies[3];
	Vector2D coords[3];
};

struct LineSegment
{
	Vector3D vertecies[2];
	Color colors[2];
};
