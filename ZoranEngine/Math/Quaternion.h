#pragma once
#include "Vector3.h"
#include "Matrix.hpp"
#include "MathLib.h"

#include "Core/PlatformTypes.h"

class ZoranEngine_EXPORT Quaternion
{
private:
	double vals[4];
public:
	Quaternion();
	Quaternion(const double* array);
	Quaternion(double x, double y, double z, double w);
	Quaternion(Vector3D complex,double w);
	~Quaternion();

	Vector3D GetComplex() const;
	Quaternion GetConjugate()const;
	Quaternion GetInverse();

	Quaternion Product(const Quaternion& rhs) const;
	double GetMagnitude() const;

	MatrixF AsMatrix() const;
	MatrixF AsRightMatrix() const;
	MatrixF AsRotationMatrix() const;
	Vector3D RotatedVector(const Vector3D& v) const;

	static Quaternion FromScaledAxis(Vector3D& w);
	static Quaternion FromEuler(const Vector3D& euler);
	static Quaternion FromEuler(double x, double y, double z);

	Vector3D AsEuler(void) const;
	// scaled linear interp
	Quaternion Slerp(const Quaternion& q1, double t);
	static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, double t);

	inline double X()const { return vals[0]; }
	inline double Y()const { return vals[1]; }
	inline double Z()const { return vals[2]; }
	inline double W()const { return vals[3]; }

	// opreators
	Quaternion operator*(const Quaternion& rhs) const;
	Quaternion operator-(const Quaternion& rhs) const;
	Quaternion operator*(double s) const;
	Quaternion operator/(double s) const;
	Quaternion operator-() const;
};

Quaternion operator*(double s, const Quaternion& q);