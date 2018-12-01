#include "stdafx.h"
#include "Vector2.h"

#include <math.h>

const Vector2D Vector2D::Zero;
const Vector2I Vector2I::Zero;

Vector2D::Vector2D(void) : x(0), y(0) { }
Vector2D::Vector2D(float x_, float y_) :x(x_), y(y_) { }

Vector2D::Vector2D(const Vector2I & other)
{
	this->x = static_cast<float>(other.x);
	this->y = static_cast<float>(other.y);
}

Vector2D::Vector2D(const Vector3D & other)
{
	x = other.x;
	y = other.y;
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

float Vector2D::getMaxValue() const
{
	return (x > y) ? x : y;
}

float Vector2D::distance(const Vector2D& other)const
{
	return sqrtf(distanceSquared(other));
}

float Vector2D::distanceSquared(const Vector2D& other)const
{
	float dx = other.x - x;
	float dy = other.y - y;

	return (dx*dx) + (dy*dy);
}

void Vector2D::normalize()
{
	float mag = magnitude();
	*this /= mag;
}

Vector2D operator-(float d, const Vector2D & v)
{
	return Vector2D(d - v.x, d - v.y);
}

inline Vector2D operator*(float d, const Vector2D& v)
{
	return v * d;
}

Vector2D operator+(float d, const Vector2D & v)
{
	return Vector2D(d + v.x, d + v.y);
}

Vector2D operator/(float d, const Vector2D & v)
{
	return Vector2D(d / v.x, d / v.y);
}

Vector2I::Vector2I(void) : x(0), y(0) { }
Vector2I::Vector2I(int x_, int y_) : x(x_), y(y_) { }

Vector2I::Vector2I(const Vector2D & other) : x((int)other.x), y((int)other.y)
{
}

Vector2I::~Vector2I(void) {}

Vector2I Vector2I::operator -(const Vector2I& other)const
{
	Vector2I ret = *this;
	ret.x -= other.x;
	ret.y -= other.y;

	return ret;
}

Vector2I Vector2I::operator +(const Vector2I& other)const
{
	Vector2I ret = *this;
	ret.x += other.x;
	ret.y += other.y;

	return ret;
}

Vector2I Vector2I::operator *(const Vector2I& other)const
{
	Vector2I ret = *this;
	ret.x *= other.x;
	ret.y *= other.y;

	return ret;
}

Vector2I Vector2I::operator*(const Vector2D& other)const
{
	return Vector2I((int)((float)this->x * other.x),(int)((float)this->y * other.y));
}

Vector2I Vector2I::operator /(const Vector2I& other)const
{
	Vector2I ret = *this;
	ret.x /= other.x;
	ret.y /= other.y;

	return ret;
}

Vector2I Vector2I::operator =(const Vector2I& other)
{
	x = other.x;
	y = other.y;

	return *this;
}

Vector2I Vector2I::operator -=(const Vector2I& other)
{
	x -= other.x;
	y -= other.y;

	return *this;
}

Vector2I Vector2I::operator +=(const Vector2I& other)
{
	x += other.x;
	y += other.y;

	return *this;
}

Vector2I Vector2I::operator *=(const Vector2I& other)
{
	x *= other.x;
	y *= other.y;

	return *this;
}

Vector2I Vector2I::operator /=(const Vector2I& other)
{
	x /= other.x;
	y /= other.y;

	return *this;
}

Vector2I Vector2I::operator -(int scalor)const
{
	Vector2I ret = *this;
	ret.x -= scalor;
	ret.y -= scalor;

	return ret;
}

Vector2I Vector2I::operator +(int scalor)const
{
	Vector2I ret = *this;
	ret.x += scalor;
	ret.y += scalor;

	return ret;
}

Vector2I Vector2I::operator *(int scalor)const
{
	Vector2I ret = *this;
	ret.x *= scalor;
	ret.y *= scalor;

	return ret;
}

Vector2I Vector2I::operator /(int scalor)const
{
	Vector2I ret = *this;
	ret.x /= scalor;
	ret.y /= scalor;

	return ret;
}

Vector2I Vector2I::operator -=(int scalor)
{
	x -= scalor;
	y -= scalor;

	return *this;
}

Vector2I Vector2I::operator +=(int scalor)
{
	x += scalor;
	y += scalor;

	return *this;
}

Vector2I Vector2I::operator *=(int scalor)
{
	x *= scalor;
	y *= scalor;

	return *this;
}

Vector2I Vector2I::operator /=(int scalor)
{
	x /= scalor;
	y /= scalor;

	return *this;
}

Vector2I Vector2I::operator =(int scalor)
{
	x = scalor;
	y = scalor;

	return *this;
}

Vector2I Vector2I::getAbs()const
{
	return Vector2I(abs(x), abs(y));
}

bool Vector2I::operator ==(const Vector2I& other)const
{
	return other.x == x && other.y == y;
}

bool Vector2I::operator<=(const Vector2I & other)const
{
	return x <= other.x && y <= other.y;
}

bool Vector2I::operator>=(const Vector2I & other)const
{
	return x >= other.x && y >= other.y;
}

float Vector2I::magnitude()const
{
	return sqrtf(static_cast<float>((x*x) + (y*y)));
}

float Vector2I::distance(const Vector2I& other)const
{
	return sqrtf(static_cast<float>(distanceSquared(other)));
}

int Vector2I::distanceSquared(const Vector2I& other)const
{
	int dx = other.x - x;
	int dy = other.y - y;

	return (dx * x) + (dy*dy);
}

int Vector2I::perpDot(const Vector2I& other)const
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
