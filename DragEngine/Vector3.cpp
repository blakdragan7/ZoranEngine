#include "stdafx.h"
#include "Vector3.h"
#include "Vector2.h"
#include <math.h>

Vector3D::Vector3D(void) :x(0), y(0), z(0) {}

Vector3D::Vector3D(Vector2D other) :x(other.x), y(other.y), z(0) {}

Vector3D::Vector3D(double x_, double y_, double z_) :x(x_), y(y_), z(z_) {}

Vector3D::Vector3D::~Vector3D(void) {}

bool Vector3D::operator ==(Vector3D other)
{
	return (x == other.x&&y == other.y&&z == other.z);
}

bool Vector3D::operator !=(Vector3D other)
{
	return !(this->operator==(other));
}

void Vector3D::operator =(Vector3D other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void Vector3D::operator =(Vector2D other)
{
	x = other.x;
	y = other.y;
}

void Vector3D::operator+=(Vector3D &other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vector3D::operator-=(Vector3D &other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void Vector3D::operator*=(Vector3D &other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

void Vector3D::operator/=(Vector3D &other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
}

void Vector3D::operator+=(double &scalor)
{
	x += scalor;
	y += scalor;
	z += scalor;
}

void Vector3D::operator-=(double &scalor)
{
	x -= scalor;
	y -= scalor;
	z -= scalor;
}

void Vector3D::operator*=(double &scalor)
{
	x *= scalor;
	y *= scalor;
	z *= scalor;
}

void Vector3D::operator/=(double &scalor)
{
	x /= scalor;
	y /= scalor;
	z /= scalor;
}

Vector3D Vector3D::operator-()
{
	return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator+(Vector3D &other)
{
	Vector3D ret(0,0,0);

	ret.x = x + other.x;
	ret.y = y + other.y;
	ret.z = z + other.z;

	return ret;
}

Vector3D Vector3D::operator-(Vector3D &other)
{
	Vector3D ret;
	ret.x = x - other.x;
	ret.y = y - other.y;
	ret.z = z - other.z;

	return ret;
}

Vector3D Vector3D::operator*(Vector3D &other)
{
	Vector3D ret;
	ret.x = x * other.x;
	ret.y = y * other.y;
	ret.z = z * other.z;

	return ret;
}

Vector3D Vector3D::operator/(Vector3D &other)
{
	Vector3D ret;
	ret.x = x / other.x;
	ret.y = y / other.y;
	ret.z = z / other.z;

	return ret;
}

Vector3D Vector3D::operator+(double scalor)
{
	Vector3D ret;
	ret.x = x + scalor;
	ret.y = y + scalor;
	ret.z = z + scalor;

	return ret;
}

Vector3D Vector3D::operator-(double scalor)
{
	Vector3D ret;
	ret.x = x - scalor;
	ret.y = y - scalor;
	ret.z = z - scalor;

	return ret;
}

Vector3D Vector3D::operator*(double scalor)
{
	Vector3D ret;
	ret.x = x * scalor;
	ret.y = y * scalor;
	ret.z = z * scalor;

	return ret;
}

Vector3D Vector3D::operator/(double scalor)
{
	Vector3D ret;
	ret.x = x / scalor;
	ret.y = y / scalor;
	ret.z = z / scalor;

	return ret;
}

double Vector3D::getMagnitude()
{
	return sqrt((x*x) + (y*y) + (z*z));
}

double Vector3D::getMagnitudeSqr()
{
	return ((x*x) + (y*y) + (z*z));
}

void Vector3D::normalize()
{
	double mag = abs(getMagnitude());

	x /= mag;
	y /= mag;
	z /= mag;
}

double Vector3D::dot(const Vector3D other)
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3D Vector3D::cross(const Vector3D other)
{
	Vector3D res;
	res.x = (y * other.z) - (other.y * z);
	res.y = (z * other.x) - (other.z * x);
	res.z = (x * other.y) - (other.x * y);

	return res;
}

Vector3D Vector3D::getAbs()
{
	return Vector3D(abs(x),abs(y),abs(z));
}

double Vector3D::distance(Vector3D other)
{
	return (other - *this).getMagnitude();
}

double Vector3D::distanceSqr(Vector3D other)
{
	return (other - *this).getMagnitudeSqr();
}

Vector3D operator/(Vector3D & other, double & scalor)
{
	return Vector3D(other.x / scalor, other.y / scalor, other.z / scalor);
}

Vector3D operator*(double d, Vector3D v)
{
	return v*d;
}
