#pragma once
#ifdef QT
#include <QVector2D>
#include <QVector3D>
#endif
#include "Vector2D.hpp"
#include <math.h>
#include "PlatformTypes.h"

template <class t>
class DRAGENGINE_EXPORT Vector3D
{
public:
	t	x,y,z;

	Vector3D(void):x(0),y(0),z(0){}

	Vector3D(Vector2D<t> other) :x(other.x), y(other.y), z(0) {}

	Vector3D(t x_,t y_,t z_):x(x_),y(y_),z(z_){}

	Vector3D::~Vector3D(void){}

#ifdef QT
	operator QVector2D() const { return QVector2D(x, y); }
	operator QVector3D() const { return QVector3D(x, y, z); }
#endif

	bool operator ==(Vector3D other)
	{
		return (x==other.x&&y==other.y&&z==other.z);
	}

	bool operator !=(Vector3D other)
	{
		return !(this->operator==(other));
	}

	void operator =(Vector3D<t> other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
	}

	void operator =(Vector2D<t> other)
	{
		x = other.x;
		y = other.y;
	}

	void operator+=(Vector3D &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator-=(Vector3D &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	void operator*=(Vector3D &other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
	}

	void operator/=(Vector3D &other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
	}

	void operator+=(t &scalor)
	{
		x += scalor;
		y += scalor;
		z += scalor;
	}

	void operator-=(t &scalor)
	{
		x -= scalor;
		y -= scalor;
		z -= scalor;
	}

	void operator*=(t &scalor)
	{
		x *= scalor;
		y *= scalor;
		z *= scalor;
	}

	void operator/=(t &scalor)
	{
		x /= scalor;
		y /= scalor;
		z /= scalor;
	}

	Vector3D operator+(Vector3D &other)
	{
		Vector3D ret;
		ret.x = x + other.x;
		ret.x = y + other.y;
		ret.x = z + other.z;

		return ret;
	}

	Vector3D operator-(Vector3D &other)
	{
		Vector3D ret;
		ret.x= x - other.x;
		ret.y= y - other.y;
		ret.z= z - other.z;
	
		return ret;
	}

	Vector3D operator*(Vector3D &other)
	{
		Vector3D ret;
		ret.x = x * other.x;
		ret.y = y * other.y;
		ret.z = z * other.z;
	
		return ret;
	}

	Vector3D operator/(Vector3D &other)
	{
		Vector3D ret;
		ret.x = x / other.x;
		ret.y = y / other.y;
		ret.z = z / other.z;
	
		return ret;
	}

	Vector3D operator+(t &scalor)
	{
		Vector3D ret;
		ret.x = x + scalor;
		ret.y = y + scalor;
		ret.z = z + scalor;
	
		return ret;
	}

	Vector3D operator-(t &scalor)
	{
		Vector3D ret;
		ret.x = x - scalor;
		ret.y = y - scalor;
		ret.z = z - scalor;
	
		return ret;
	}

	Vector3D operator*(t &scalor)
	{
		Vector3D ret;
		ret.x = x * scalor;
		ret.y = y * scalor;
		ret.z = z * scalor;
	
		return ret;
	}

	Vector3D operator/(t &scalor)
	{
		Vector3D ret;
		ret.x = x / scalor;
		ret.y = y / scalor;
		ret.z = z / scalor;
	
		return ret;
	}

	double getMagnitude()
	{
		return sqrt((x*x) + (y*y) + (z*z));
	}

	void normalize()
	{
		double mag = abs(getMagnitude());

		x/=(t)mag;
		y/=(t)mag;
		z/=(t)mag;
	}

	Vector3D cross(const Vector3D other)
	{
		Vector3D res;
		res.x = (y * other.z) - (other.y * z);
		res.y = (z * other.x) - (other.z * x);
		res.z = (x * other.y) - (other.x * y);

		return res;
	}
};

typedef Vector3D<int> Vec3I;
typedef Vector3D<float> Vec3F;
typedef Vector3D<double> Vec3D;
