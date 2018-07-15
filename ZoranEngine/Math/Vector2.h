#pragma once
#include "Core/PlatformTypes.h"

class ZoranEngine_EXPORT Vector2D
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

	Vector2D(void);
	Vector2D(double x_, double y_);

	~Vector2D(void);

	Vector2D operator -()const;
	Vector2D operator -(const Vector2D& other)const;
	Vector2D operator +(const Vector2D& other)const;
	Vector2D operator *(const Vector2D& other)const;
	Vector2D operator /(const Vector2D& other)const;
	Vector2D operator =(const Vector2D& other);
	void operator -=(Vector2D other);
	void operator +=(Vector2D other);
	void operator *=(Vector2D other);
	void operator /=(Vector2D other);

	Vector2D operator -(double scalor)const;
	Vector2D operator +(double scalor)const;
	Vector2D operator *(double scalor)const;
	Vector2D operator /(double scalor)const;
	void operator -=(double scalor);
	void operator +=(double scalor);
	void operator *=(double scalor);
	void operator /=(double scalor);
	void operator =(double scalor);

	bool operator ==(const Vector2D& other)const;

	Vector2D getAbs()const;
	Vector2D getNormal()const;

	void clear();
	void normalize();
	void rotate(Vector2D origin, double angle /*radians*/);

	double magnitude()const;
	double magnitudeSqr()const;
	double distance(const Vector2D& other)const;
	double distanceSquared(const Vector2D& other)const;
	double dot(Vector2D other)const;
	double cross(Vector2D other)const;
};

Vector2D operator*(double, const Vector2D&);

class ZoranEngine_EXPORT Vector2L
{
public:
	union {
		long long x;
		long long w;
	};
	union {
		long long y;
		long long h;
	};

	Vector2L(void);
	Vector2L(long long x_, long long y_);

	~Vector2L(void);

	Vector2L operator -(Vector2L other);
	Vector2L operator +(Vector2L other);
	Vector2L operator *(Vector2L other);
	Vector2L operator /(Vector2L other);
	Vector2L operator =(Vector2L other);
	Vector2L operator -=(Vector2L other);
	Vector2L operator +=(Vector2L other);
	Vector2L operator *=(Vector2L other);
	Vector2L operator /=(Vector2L other);

	Vector2L operator -(long long scalor);
	Vector2L operator +(long long scalor);
	Vector2L operator *(long long scalor);
	Vector2L operator /(long long scalor);
	Vector2L operator -=(long long scalor);
	Vector2L operator +=(long long scalor);
	Vector2L operator *=(long long scalor);
	Vector2L operator /=(long long scalor);
	Vector2L operator =(long long scalor);

	Vector2D getAbs();

	bool operator ==(Vector2L other);

	double distance(Vector2L other);

	long long magnitude();
	long long distanceSquared(Vector2L other);
	long long perpDot(Vector2L other);
	void rotate(Vector2L origin, float angle /*radians*/);
};

#define Vec2D Vector2D
#define Vec2L Vector2L