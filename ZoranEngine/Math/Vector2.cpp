#include "stdafx.h"
#include "Vector2.h"

#include <math.h>

Vector2D::Vector2D(void) : x(0), y(0) { }
Vector2D::Vector2D(float x_, float y_) :x(x_), y(y_) { }

Vector2D::Vector2D(const Vector2L & other)
{
	this->x = other.x;
	this->y = other.y;
}

Vector2D::~Vector2D(void) {}

void Vector2D::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vector2D::Set(const Vector2D & other)
{
	x = other.x;
	y = other.y;
}

Vector2D Vector2D::operator-()const
{
	return Vector2D(-x, -y);
}

Vector2D Vector2D::operator -(const Vector2D& other)const
{
	Vector2D ret = *this;
	ret.x -= other.x;
	ret.y -= other.y;

	return ret;
}

Vector2D Vector2D::operator +(const Vector2D& other)const
{
	Vector2D ret = *this;
	ret.x += other.x;
	ret.y += other.y;

	return ret;
}

Vector2D Vector2D::operator *(const Vector2D& other)const
{
	Vector2D ret = *this;
	ret.x *= other.x;
	ret.y *= other.y;

	return ret;
}

Vector2D Vector2D::operator /(const Vector2D& other)const
{
	Vector2D ret = *this;
	ret.x /= other.x;
	ret.y /= other.y;

	return ret;
}

Vector2D Vector2D::operator =(const Vector2D& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

void Vector2D::operator -=(Vector2D other)
{
	x -= other.x;
	y -= other.y;
}

void Vector2D::operator +=(Vector2D other)
{
	x += other.x;
	y += other.y;
}

void Vector2D::operator *=(Vector2D other)
{
	x *= other.x;
	y *= other.y;
}

void Vector2D::operator /=(Vector2D other)
{
	x /= other.x;
	y /= other.y;
}

Vector2D Vector2D::operator -(float scalor)const
{
	Vector2D ret = *this;
	ret.x -= scalor;
	ret.y -= scalor;

	return ret;
}

Vector2D Vector2D::operator +(float scalor)const
{
	Vector2D ret = *this;
	ret.x += scalor;
	ret.y += scalor;

	return ret;
}

Vector2D Vector2D::operator *(float scalor)const
{
	Vector2D ret = *this;
	ret.x *= scalor;
	ret.y *= scalor;

	return ret;
}

Vector2D Vector2D::operator /(float scalor)const
{
	Vector2D ret = *this;
	ret.x /= scalor;
	ret.y /= scalor;

	return ret;
}

void Vector2D::operator -=(float scalor)
{
	x -= scalor;
	y -= scalor;
}

void Vector2D::operator +=(float scalor)
{
	x += scalor;
	y += scalor;
}

void Vector2D::operator *=(float scalor)
{
	x *= scalor;
	y *= scalor;
}

void Vector2D::operator /=(float scalor)
{
	x /= scalor;
	y /= scalor;
}

void Vector2D::operator =(float scalor)
{
	x = scalor;
	y = scalor;
}

bool Vector2D::operator ==(const Vector2D& other)const
{
	return other.x == x && other.y == y;
}

Vector2D Vector2D::getAbs()const
{
	return Vector2D(abs(x),abs(y));
}

Vector2D Vector2D::getNormal()const
{
	Vector2D ret = *this;
	ret.normalize();
	return ret;
}

void Vector2D::clear()
{
	x = 0;
	y = 0;
}

float Vector2D::dot(const Vector2D& other)const
{
	return (x*other.x) + (y*other.y);
}

float Vector2D::cross(const Vector2D& other)const
{
	return (x*other.y) - (y*other.x);
}

Vector2D Vector2D::crossRight(float scalor) const
{
	return Vector2D(scalor * y, -scalor * x);
}

Vector2D Vector2D::crossLeft(float scalor) const
{
	return Vector2D(-scalor * y, scalor * x);
}

void Vector2D::rotate(Vector2D origin, float angle /*radians*/)
{
	Vector2D p(*this);

	float s = sinf(angle);
	float c = cosf(angle);

	// translate point back to origin:
	p.x -= origin.x;
	p.y -= origin.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + origin.x;
	p.y = ynew + origin.y;

	x = p.x;
	y = p.y;
}

float Vector2D::magnitudeSqr()const
{
	return (x*x) + (y*y);
}

float Vector2D::magnitude()const
{
	return sqrt(magnitudeSqr());
}

float Vector2D::distance(const Vector2D& other)const
{
	float dx = other.x - x;
	float dy = other.y - y;

	return sqrt(pow(dx, 2) + pow(dy, 2));
}

float Vector2D::distanceSquared(const Vector2D& other)const
{
	float dx = other.x - x;
	float dy = other.y - y;

	return (pow(dx, 2) + pow(dy, 2));
}

void Vector2D::normalize()
{
	float mag = magnitude();
	*this /= mag;
}

inline Vector2D operator*(float d, const Vector2D& v)
{
	return v * d;
}

Vector2L::Vector2L(void) : x(0), y(0) { }
Vector2L::Vector2L(long long x_, long long y_) : x(x_), y(y_) { }

Vector2L::~Vector2L(void) {}

Vector2L Vector2L::operator -(Vector2L other)
{
	Vector2L ret = *this;
	ret.x -= other.x;
	ret.y -= other.y;

	return ret;
}

Vector2L Vector2L::operator +(Vector2L other)
{
	Vector2L ret = *this;
	ret.x += other.x;
	ret.y += other.y;

	return ret;
}

Vector2L Vector2L::operator *(Vector2L other)
{
	Vector2L ret = *this;
	ret.x *= other.x;
	ret.y *= other.y;

	return ret;
}

Vector2L Vector2L::operator /(Vector2L other)
{
	Vector2L ret = *this;
	ret.x /= other.x;
	ret.y /= other.y;

	return ret;
}

Vector2L Vector2L::operator =(Vector2L other)
{
	x = other.x;
	y = other.y;

	return *this;
}

Vector2L Vector2L::operator -=(Vector2L other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2L Vector2L::operator +=(Vector2L other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2L Vector2L::operator *=(Vector2L other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2L Vector2L::operator /=(Vector2L other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

Vector2L Vector2L::operator -(long long scalor)
{
	Vector2L ret = *this;
	ret.x -= scalor;
	ret.y -= scalor;

	return ret;
}

Vector2L Vector2L::operator +(long long scalor)
{
	Vector2L ret = *this;
	ret.x += scalor;
	ret.y += scalor;

	return ret;
}

Vector2L Vector2L::operator *(long long scalor)
{
	Vector2L ret = *this;
	ret.x *= scalor;
	ret.y *= scalor;

	return ret;
}

Vector2L Vector2L::operator /(long long scalor)
{
	Vector2L ret = *this;
	ret.x /= scalor;
	ret.y /= scalor;

	return ret;
}

Vector2L Vector2L::operator -=(long long scalor)
{
	x -= scalor;
	y -= scalor;

	return *this;
}

Vector2L Vector2L::operator +=(long long scalor)
{
	x += scalor;
	y += scalor;

	return *this;
}

Vector2L Vector2L::operator *=(long long scalor)
{
	x *= scalor;
	y *= scalor;

	return *this;
}

Vector2L Vector2L::operator /=(long long scalor)
{
	x /= scalor;
	y /= scalor;

	return *this;
}

Vector2L Vector2L::operator =(long long scalor)
{
	x = scalor;
	y = scalor;

	return *this;
}

Vector2D Vector2L::getAbs()
{
	return Vector2D(abs(x), abs(y));
}

bool Vector2L::operator ==(Vector2L other)
{
	return other.x == x && other.y == y;
}

void Vector2L::rotate(Vector2L origin, float angle /*radians*/)
{
	Vector2L p(*this);

	float s = sinf(angle);
	float c = cosf(angle);

	// translate point back to origin:
	p.x -= origin.x;
	p.y -= origin.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + origin.x;
	p.y = ynew + origin.y;

	x = p.x;
	y = p.y;
}

long long Vector2L::magnitude()
{
	return sqrt((x*x) + (y*y));
}

float Vector2L::distance(Vector2L other)
{
	long long dx = other.x - x;
	long long dy = other.y - y;

	return sqrt(pow(dx, 2) + pow(dy, 2));
}

long long Vector2L::distanceSquared(Vector2L other)
{
	long long dx = other.x - x;
	long long dy = other.y - y;

	return (pow(dx, 2) + pow(dy, 2));
}

long long Vector2L::perpDot(Vector2L other)
{
	return (x*other.y) - (y*other.x);
}


Vector2D operator/(Vector2D & other, float & scalor)
{
	return Vector2D(other.x / scalor, other.y / scalor);
}

Vector2D operator*(float d, Vector2D v)
{
	return v * d;
}
