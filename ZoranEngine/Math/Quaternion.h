#pragma once
#include "Vector3.h"
#include "Matrix.hpp"
#include "MathLib.h"

#include "Core/PlatformTypes.h"

class ZoranEngine_EXPORT Quaternion
{
private:
	float vals[4];
public:
	Quaternion();
	Quaternion(const float* array);
	Quaternion(float x, float y, float z, float w);
	Quaternion(Vector3D complex,float w);
	~Quaternion();

	Vector3D GetComplex() const;
	Quaternion GetConjugate()const;
	Quaternion GetInverse();

	Quaternion Product(const Quaternion& rhs) const;
	float GetMagnitude() const;

	MatrixF AsMatrix() const;
	MatrixF AsRightMatrix() const;
	MatrixF AsRotationMatrix() const;
	Vector3D RotatedVector(const Vector3D& v) const;

	static Quaternion FromScaledAxis(Vector3D& w);
	static Quaternion FromEuler(const Vector3D& euler);
	static Quaternion FromEuler(float x, float y, float z);

	Vector3D AsEuler(void) const;
	// scaled linear interp
	Quaternion Slerp(const Quaternion& q1, float t);
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

	inline float X()const { return vals[0]; }
	inline float Y()const { return vals[1]; }
	inline float Z()const { return vals[2]; }
	inline float W()const { return vals[3]; }

	// opreators
	Quaternion operator*(const Quaternion& rhs) const;
	Quaternion operator-(const Quaternion& rhs) const;
	Quaternion operator*(float s) const;
	Quaternion operator/(float s) const;
	Quaternion operator-() const;
};

Quaternion operator*(float s, const Quaternion& q);
