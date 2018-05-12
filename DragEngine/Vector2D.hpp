#pragma once
#include "PlatformTypes.h"
static unsigned long long IITD = 0;

template <class t>
class DRAGENGINE_EXPORT Vector2D
{
private:
	//unsigned long long _IITD;
public:
	union {
		t x;
		t w;
	};
	union {
		t y;
		t h;
	};

	Vector2D(void) : x(0), y(0) { /*_IITD = IITD++;*/ }
	Vector2D(t x_,t y_):x(x_),y(y_) { /*_IITD = IITD++;*/ }

	~Vector2D(void){}

	Vector2D operator -(Vector2D other)
	{
		Vector2D ret = *this;
		ret.x -= other.x;
		ret.y -= other.y;

		return ret;
	}

	Vector2D operator +(Vector2D other)
	{
		Vector2D ret = *this;
		ret.x += other.x;
		ret.y += other.y;

		return ret;
	}

	Vector2D operator *(Vector2D other)
	{
		Vector2D ret = *this;
		ret.x *= other.x;
		ret.y *= other.y;

		return ret;
	}

	Vector2D operator /(Vector2D other)
	{
		Vector2D ret = *this;
		ret.x /= other.x;
		ret.y /= other.y;

		return ret;
	}

	Vector2D operator =(Vector2D other)
	{
		x = other.x;
		y = other.y;

		return *this;
	}

	Vector2D operator -=(Vector2D other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}

	Vector2D operator +=(Vector2D other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2D operator *=(Vector2D other)
	{
		x *= other.x;
		y *= other.y;

		return *this;
	}

	Vector2D operator /=(Vector2D other)
	{
		x /= other.x;
		y /= other.y;

		return *this;
	}

	Vector2D operator -(t scalor)
	{
		Vector2D ret = *this;
		ret.x -= scalor;
		ret.y -= scalor;

		return ret;
	}

	Vector2D operator +(t scalor)
	{
		Vector2D ret = *this;
		ret.x += scalor;
		ret.y += scalor;

		return ret;
	}

	Vector2D operator *(t scalor)
	{
		Vector2D ret = *this;
		ret.x *= scalor;
		ret.y *= scalor;

		return ret;
	}

	Vector2D operator /(t scalor)
	{
		Vector2D ret = *this;
		ret.x /= scalor;
		ret.y /= scalor;

		return ret;
	}

	Vector2D operator -=(t scalor)
	{
		x -= scalor;
		y -= scalor;

		return *this;
	}

	Vector2D operator +=(t scalor)
	{
		x += scalor;
		y += scalor;

		return *this;
	}

	Vector2D operator *=(t scalor)
	{
		x *= scalor;
		y *= scalor;

		return *this;
	}

	Vector2D operator /=(t scalor)
	{
		x /= scalor;
		y /= scalor;

		return *this;
	}

	Vector2D operator =(t scalor)
	{
		x = scalor;
		y = scalor;

		return *this;
	}

	bool operator ==(Vector2D other)
	{
		return /*_IITD == other._IITD &&*/ other.x == x && other.y == y;
	}

	void rotate(Vector2D origin,float angle /*radians*/)
	{
		Vector2D p(*this);
	  
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

	t magnitude()
	{
		return sqrt((x*x) + (y*y));
	}

	double distance(Vector2D other)
	{
		float dx = other.x - x;
		float dy = other.y-y;

		return sqrt(pow(dx,2) + pow(dy,2));
	}

	double distanceSquared(Vector2D other)
	{
		float dx = other.x - x;
		float dy = other.y - y;

		return (pow(dx, 2) + pow(dy, 2));
	}

	void normalize()
	{
		t mag = magnitude();
		*this /= mag;
	}
};

typedef Vector2D<int>		Vec2I;
typedef Vector2D<long>		Vec2L;
typedef Vector2D<float>		Vec2F;
typedef Vector2D<double>	Vec2D;