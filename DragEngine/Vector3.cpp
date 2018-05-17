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
	Vector3D ret;
	ret.x = x + other.x;
	ret.x = y + other.y;
	ret.x = z + other.z;

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

Vector3D Vector3D::operator+(double &scalor)
{
	Vector3D ret;
	ret.x = x + scalor;
	ret.y = y + scalor;
	ret.z = z + scalor;

	return ret;
}

Vector3D Vector3D::operator-(double &scalor)
{
	Vector3D ret;
	ret.x = x - scalor;
	ret.y = y - scalor;
	ret.z = z - scalor;

	return ret;
}

Vector3D Vector3D::operator*(double &scalor)
{
	Vector3D ret;
	ret.x = x * scalor;
	ret.y = y * scalor;
	ret.z = z * scalor;

	return ret;
}

Vector3D Vector3D::operator/(double &scalor)
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

void Vector3D::normalize()
{
	double mag = abs(getMagnitude());

	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3D Vector3D::cross(const Vector3D other)
{
	Vector3D res;
	res.x = (y * other.z) - (other.y * z);
	res.y = (z * other.x) - (other.z * x);
	res.z = (x * other.y) - (other.x * y);

	return res;
}