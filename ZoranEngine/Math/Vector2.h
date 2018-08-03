#pragma once
#include "Core/PlatformTypes.h"

class ZoranEngine_EXPORT Vector2D
{
public:
	union {
		float x;
		float w;
	};
	union {
		float y;
		float h;
	};

	Vector2D(void);
	Vector2D(float x_, float y_);
	Vector2D(const class Vector2L& other);

	~Vector2D(void);

	void Set(float x, float y);
	void Set(const Vector2D& other);

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

	Vector2D operator -(float scalor)const;
	Vector2D operator +(float scalor)const;
	Vector2D operator *(float scalor)const;
	Vector2D operator /(float scalor)const;
	void operator -=(float scalor);
	void operator +=(float scalor);
	void operator *=(float scalor);
	void operator /=(float scalor);
	void operator =(float scalor);

	bool operator ==(const Vector2D& other)const;

	Vector2D getAbs()const;
	Vector2D getNormal()const;

	void clear();
	void normalize();
	void rotate(Vector2D origin, float angle /*radians*/);

	float magnitude()const;
	float magnitudeSqr()const;
	float distance(const Vector2D& other)const;
	float distanceSquared(const Vector2D& other)const;
	float dot(const Vector2D& other)const;
	float cross(const Vector2D& other)const;
	Vector2D crossRight(float scalor)const;
	Vector2D crossLeft(float scalor)const;
};

inline extern Vector2D operator*(float d,const Vector2D& v);

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

	float distance(Vector2L other);

	long long magnitude();
	long long distanceSquared(Vector2L other);
	long long perpDot(Vector2L other);
	void rotate(Vector2L origin, float angle /*radians*/);
};

#define Vec2D Vector2D
#define Vec2L Vector2L