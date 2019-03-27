#pragma once

#include <Core/PlatformTypes.h>

class Vector2D;
class Color;
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
	Vector3D(const Vector2D& other);
	Vector3D(const Vector2D& other, float z);
	Vector3D(const Color& other);
	Vector3D(float x_, float y_, float z_);
	~Vector3D(void);

	bool operator ==(const Vector3D& other)const;
	bool operator !=(const Vector3D& other)const;
	void operator =(const Vector2D& other);
	void operator =(const Vector3D& other);

	void operator +=(const Vector3D& other);
	void operator -=(const Vector3D& other);
	void operator *=(const Vector3D& other);
	void operator /=(const Vector3D& other);

	void operator +=(float scalor);
	void operator -=(float scalor);
	void operator *=(float scalor);
	void operator /=(float scalor);

	Vector3D operator -()const;
	Vector3D operator +(const Vector3D &other)const;
	Vector3D operator -(const Vector3D &other)const;
	Vector3D operator *(const Vector3D &other)const;
	Vector3D operator /(const Vector3D &other)const;
	Vector3D operator +(float scalor)const;
	Vector3D operator -(float scalor)const;
	Vector3D operator *(float scalor)const;
	Vector3D operator /(float scalor)const;

	bool nearlyEquals(float other)const;
	bool nearlyEquals(const Vector3D& other)const;

	float getMagnitude()const;
	float getMagnitudeSqr()const;
	void normalize();

	float dot(const Vector3D& other)const;
	Vector3D cross(const Vector3D& other)const;
	Vector3D getAbs()const;

	Vector2D GetXY()const;
	Vector2D GetYZ()const;
	Vector2D GetXZ()const;

	float distance(const Vector3D& other)const;
	float distanceSqr(const Vector3D& other)const;
};

#define Vec3D const Vector3D&

Vector3D operator*(float, const Vector3D&);
