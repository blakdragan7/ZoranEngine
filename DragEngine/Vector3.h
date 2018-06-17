#pragma once

#include "PlatformTypes.h"

class Vector2D;
class DRAGENGINE_EXPORT Vector3D
{
public:
	union {
		double x;
		double w;
	};
	
	union {
		double y;
		double h;
	};

	union {
		double z;
		double d;
	};

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

	bool nearlyEquals(double other);
	bool nearlyEquals(Vector3D other);

	double getMagnitude();
	double getMagnitudeSqr();
	void normalize();

	double dot(const Vector3D other);
	Vector3D cross(const Vector3D other);
	Vector3D getAbs();

	double distance(Vector3D other);
	double distanceSqr(Vector3D other);
};

#define Vec3D Vector3D

Vector3D operator*(double, Vector3D);
