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
	Vector2D(const class Vector2I& other);
	Vector2D(const class Vector3D& other); // this just ignores the z component

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

	static const Vector2D Zero;
};

inline extern Vector2D operator*(float d,const Vector2D& v);

class ZoranEngine_EXPORT Vector2I
{
public:
	union {
		int x;
		int w;
	};
	union {
		int y;
		int h;
	};

	Vector2I(void);
	Vector2I(int x_, int y_);

	~Vector2I(void);

	Vector2I operator -(Vector2I other);
	Vector2I operator +(Vector2I other);
	Vector2I operator *(Vector2I other);
	Vector2I operator /(Vector2I other);
	Vector2I operator =(Vector2I other);
	Vector2I operator -=(Vector2I other);
	Vector2I operator +=(Vector2I other);
	Vector2I operator *=(Vector2I other);
	Vector2I operator /=(Vector2I other);

	Vector2I operator -(int scalor);
	Vector2I operator +(int scalor);
	Vector2I operator *(int scalor);
	Vector2I operator /(int scalor);
	Vector2I operator -=(int scalor);
	Vector2I operator +=(int scalor);
	Vector2I operator *=(int scalor);
	Vector2I operator /=(int scalor);
	Vector2I operator =(int scalor);

	Vector2I getAbs();

	bool operator ==(Vector2I other);

	float distance(Vector2I other);

	float magnitude();
	int distanceSquared(Vector2I other);
	int perpDot(Vector2I other);

};


#define Vec2D Vector2D
#define Vec2I Vector2I