#pragma once
#include <Math/Vector3.h>
#include <Math/Vector2.h>

class ZoranEngine_EXPORT Color
{
public:
	float r, g, b, a;

	static const Color Black;
	static const Color Red;
	static const Color Blue;
	static const Color Green;
	static const Color White;
	static const Color Transparent;
	static const Color Clear;

	Color() : r(0), g(0), b(0), a(0) {}
	Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
};

struct ZoranEngine_EXPORT RectanglePrimitive
{
	Vector3D vertecies[4];
	Vector2D coords[4];
};

struct ZoranEngine_EXPORT TrianglePrimitive
{
	Vector3D vertecies[3];
	Vector2D coords[3];
};

struct ZoranEngine_EXPORT LineSegment
{
	Vector3D vertecies[2];
	Color colors[2];
};
