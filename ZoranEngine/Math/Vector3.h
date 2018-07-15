#pragma once

#include "PlatformTypes.h"

class Vector2D;
class ZoranEngine_EXPORT Vector3D
{
public:
	union {
		float x;
		float w;
	};
	
	union {
		float y;
		float h;
	};

	union {
		float z;
		float d;
	};

	Vector3D(void);
	Vector3D(Vector2D other);
	Vector3D(float x_, float y_, float z_);
	Vector3D::~Vector3D(void);

	bool operator ==(Vector3D other);
	bool operator !=(Vector3D other);
	void operator =(Vector3D other);
	void operator =(Vector2D other);

	void operator +=(Vector3D &other);
	void operator -=(Vector3D &other);
	void operator *=(Vector3D &other);
	void operator /=(Vector3D &other);

	void operator +=(float scalor);
	void operator -=(float scalor);
	void operator *=(float scalor);
	void operator /=(float scalor);

	Vector3D operator -();
	Vector3D operator +(Vector3D &other);
	Vector3D operator -(Vector3D &other);
	Vector3D operator *(Vector3D &other);
	Vector3D operator /(Vector3D &other);
	Vector3D operator +(float scalor);
	Vector3D operator -(float scalor);
	Vector3D operator *(float scalor);
	Vector3D operator /(float scalor);

	bool nearlyEquals(float other);
	bool nearlyEquals(Vector3D other);

	float getMagnitude();
	float getMagnitudeSqr();
	void normalize();

	float dot(const Vector3D other);
	Vector3D cross(const Vector3D other);
	Vector3D getAbs();

	Vector2D GetXY();
	Vector2D GetYZ();
	Vector2D GetXZ();

	float distance(Vector3D other);
	float distanceSqr(Vector3D other);
};

#define Vec3D Vector3D

Vector3D operator*(float, Vector3D);
