#include "stdafx.h"
#include "Vector3.h"
#include "Vector2.h"
#include <math.h>

Vector3D::Vector3D(void) :x(0), y(0), z(0) {}

Vector3D::Vector3D(const Vector2D& other) :x(other.x), y(other.y), z(0) {}

Vector3D::Vector3D(float x_, float y_, float z_) :x(x_), y(y_), z(z_) {}

Vector3D::Vector3D::~Vector3D(void) {}

bool Vector3D::operator ==(const Vector3D& other)const
{
	return (x == other.x&&y == other.y&&z == other.z);
}

bool Vector3D::operator !=(const Vector3D& other)const
{
	return !(this->operator==(other));
}

void Vector3D::operator =(const Vector3D& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
}

void Vector3D::operator =(const Vector2D& other)
{
	x = other.x;
	y = other.y;
}

void Vector3D::operator+=(const Vector3D& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

void Vector3D::operator-=(const Vector3D& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

void Vector3D::operator*=(const Vector3D& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

void Vector3D::operator/=(const Vector3D& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
}

void Vector3D::operator+=(float scalor)
{
	x += scalor;
	y += scalor;
	z += scalor;
}

void Vector3D::operator-=(float scalor)
{
	x -= scalor;
	y -= scalor;
	z -= scalor;
}

void Vector3D::operator*=(float scalor)
{
	x *= scalor;
	y *= scalor;
	z *= scalor;
}

void Vector3D::operator/=(float scalor)
{
	x /= scalor;
	y /= scalor;
	z /= scalor;
}

Vector3D Vector3D::operator-()const
{
	return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator+(const Vector3D &other)const
{
	Vector3D ret(0,0,0);

	ret.x = x + other.x;
	ret.y = y + other.y;
	ret.z = z + other.z;

	return ret;
}

Vector3D Vector3D::operator-(const Vector3D &other)const
{
	Vector3D ret;
	ret.x = x - other.x;
	ret.y = y - other.y;
	ret.z = z - other.z;

	return ret;
}

Vector3D Vector3D::operator*(const Vector3D &other)const
{
	Vector3D ret;
	ret.x = x * other.x;
	ret.y = y * other.y;
	ret.z = z * other.z;

	return ret;
}

Vector3D Vector3D::operator/(const Vector3D &other)const
{
	Vector3D ret;
	ret.x = x / other.x;
	ret.y = y / other.y;
	ret.z = z / other.z;

	return ret;
}

Vector3D Vector3D::operator+(float scalor)const
{
	Vector3D ret;
	ret.x = x + scalor;
	ret.y = y + scalor;
	ret.z = z + scalor;

	return ret;
}

Vector3D Vector3D::operator-(float scalor)const
{
	Vector3D ret;
	ret.x = x - scalor;
	ret.y = y - scalor;
	ret.z = z - scalor;

	return ret;
}

Vector3D Vector3D::operator*(float scalor)const
{
	Vector3D ret;
	ret.x = x * scalor;
	ret.y = y * scalor;
	ret.z = z * scalor;

	return ret;
}

Vector3D Vector3D::operator/(float scalor)const
{
	Vector3D ret;
	ret.x = x / scalor;
	ret.y = y / scalor;
	ret.z = z / scalor;

	return ret;
}


bool Vector3D::nearlyEquals(float other)const
{
	return abs((getMagnitudeSqr() - (other*other)) < 0.00001);
}

bool Vector3D::nearlyEquals(const Vector3D& other)const
{
	return abs((getMagnitudeSqr() - (other.getMagnitudeSqr())) < 0.00001);;
}

float Vector3D::getMagnitude()const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

float Vector3D::getMagnitudeSqr()const
{
	return ((x*x) + (y*y) + (z*z));
}

void Vector3D::normalize()
{
	float mag = abs(getMagnitude());

	x /= mag;
	y /= mag;
	z /= mag;
}

float Vector3D::dot(const Vector3D& other)const
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3D Vector3D::cross(const Vector3D& other)const
{
	Vector3D res;

	res.x = (y * other.z) - (other.y * z);
	res.y = (z * other.x) - (other.z * x);
	res.z = (x * other.y) - (other.x * y);

	return res;
}

Vector3D Vector3D::getAbs()const
{
	return Vector3D(abs(x),abs(y),abs(z));
}

Vector2D Vector3D::GetXY()const
{
	return Vector2D(x,y);
}

Vector2D Vector3D::GetYZ()const
{
	return Vector2D(y,z);
}

Vector2D Vector3D::GetXZ()const
{
	return Vector2D(x,z);
}

float Vector3D::distance(const Vector3D& other)const
{
	return (other - *this).getMagnitude();
}

float Vector3D::distanceSqr(const Vector3D& other)const
{
	return (other - *this).getMagnitudeSqr();
}

Vector3D operator/(Vector3D & other, float & scalor)
{
	return Vector3D(other.x / scalor, other.y / scalor, other.z / scalor);
}

Vector3D operator*(float d, const Vector3D& v)
{
	return v*d;
}
