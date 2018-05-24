#pragma once

#include "PlatformTypes.h"

class Vector2D;
class DRAGENGINE_EXPORT Vector3D
{
public:
	double	x,y,z;

	Vector3D(void);
	Vector3D(Vector2D other);
	Vector3D(double x_, double y_, double z_);
	Vector3D::~Vector3D(void);

	bool operator ==(Vector3D other);
	bool operator !=(Vector3D other);
	void operator =(Vector3D other);
	void operator =(Vector2D other);
	void operator+=(Vector3D &other);
	void operator-=(Vector3D &other);
	void operator*=(Vector3D &other);
	void operator/=(Vector3D &other);
	void operator+=(double &scalor);
	void operator-=(double &scalor);
	void operator*=(double &scalor);
	void operator/=(double &scalor);

	Vector3D operator-();
	Vector3D operator+(Vector3D &other);
	Vector3D operator-(Vector3D &other);
	Vector3D operator*(Vector3D &other);
	Vector3D operator/(Vector3D &other);
	Vector3D operator+(double scalor);
	Vector3D operator-(double scalor);
	Vector3D operator*(double scalor);
	Vector3D operator/(double scalor);

	double getMagnitude();
	double getMagnitudeSqr();
	void normalize();

	Vector3D cross(const Vector3D other);

	double distance(Vector3D other);
	double distanceSqr(Vector3D other);
};

#define Vec3D Vector3D
