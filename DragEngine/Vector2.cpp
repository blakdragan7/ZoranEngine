#include "stdafx.h"
#include "Vector2.h"

#include <math.h>

Vector2D::Vector2D(void) : x(0), y(0) { }
Vector2D::Vector2D(double x_, double y_) :x(x_), y(y_) { }

Vector2D::~Vector2D(void) {}

Vector2D Vector2D::operator -(Vector2D other)
{
	Vector2D ret = *this;
	ret.x -= other.x;
	ret.y -= other.y;

	return ret;
}

Vector2D Vector2D::operator +(Vector2D other)
{
	Vector2D ret = *this;
	ret.x += other.x;
	ret.y += other.y;

	return ret;
}

Vector2D Vector2D::operator *(Vector2D other)
{
	Vector2D ret = *this;
	ret.x *= other.x;
	ret.y *= other.y;

	return ret;
}

Vector2D Vector2D::operator /(Vector2D other)
{
	Vector2D ret = *this;
	ret.x /= other.x;
	ret.y /= other.y;

	return ret;
}

Vector2D Vector2D::operator =(Vector2D other)
{
	x = other.x;
	y = other.y;

	return *this;
}

Vector2D Vector2D::operator -=(Vector2D other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2D Vector2D::operator +=(Vector2D other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2D Vector2D::operator *=(Vector2D other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2D Vector2D::operator /=(Vector2D other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

Vector2D Vector2D::operator -(double scalor)
{
	Vector2D ret = *this;
	ret.x -= scalor;
	ret.y -= scalor;

	return ret;
}

Vector2D Vector2D::operator +(double scalor)
{
	Vector2D ret = *this;
	ret.x += scalor;
	ret.y += scalor;

	return ret;
}

Vector2D Vector2D::operator *(double scalor)
{
	Vector2D ret = *this;
	ret.x *= scalor;
	ret.y *= scalor;

	return ret;
}

Vector2D Vector2D::operator /(double scalor)
{
	Vector2D ret = *this;
	ret.x /= scalor;
	ret.y /= scalor;

	return ret;
}

Vector2D Vector2D::operator -=(double scalor)
{
	x -= scalor;
	y -= scalor;

	return *this;
}

Vector2D Vector2D::operator +=(double scalor)
{
	x += scalor;
	y += scalor;

	return *this;
}

Vector2D Vector2D::operator *=(double scalor)
{
	x *= scalor;
	y *= scalor;

	return *this;
}

Vector2D Vector2D::operator /=(double scalor)
{
	x /= scalor;
	y /= scalor;

	return *this;
}

Vector2D Vector2D::operator =(double scalor)
{
	x = scalor;
	y = scalor;

	return *this;
}

bool Vector2D::operator ==(Vector2D other)
{
	return other.x == x && other.y == y;
}

void Vector2D::rotate(Vector2D origin, double angle /*radians*/)
{
	Vector2D p(*this);

	double s = sin(angle);
	double c = cos(angle);

	// translate point back to origin:
	p.x -= origin.x;
	p.y -= origin.y;

	// rotate point
	double xnew = p.x * c - p.y * s;
	double ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + origin.x;
	p.y = ynew + origin.y;

	x = p.x;
	y = p.y;
}

double Vector2D::magnitude()
{
	return sqrt((x*x) + (y*y));
}

double Vector2D::distance(Vector2D other)
{
	double dx = other.x - x;
	double dy = other.y - y;

	return sqrt(pow(dx, 2) + pow(dy, 2));
}

double Vector2D::distanceSquared(Vector2D other)
{
	double dx = other.x - x;
	double dy = other.y - y;

	return (pow(dx, 2) + pow(dy, 2));
}

void Vector2D::normalize()
{
	double mag = magnitude();
	*this /= mag;
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

bool Vector2L::operator ==(Vector2L other)
{
	return other.x == x && other.y == y;
}

void Vector2L::rotate(Vector2L origin, float angle /*radians*/)
{
	Vector2L p(*this);

	float s = sin(angle);
	float c = cos(angle);

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

double Vector2L::distance(Vector2L other)
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