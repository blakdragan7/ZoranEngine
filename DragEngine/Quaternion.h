#pragma once
#include "Vector3.h"
#include "Matrix.hpp"
#include "MathLib.h"

class Quaternion
{
private:
	double vals[4];
public:
	Quaternion();
	Quaternion(const double* array);
	Quaternion(double x, double y, double z, double w);
	Quaternion(Vector3D complex,double w);
	~Quaternion();

	Vector3D Complex() const;
	Quaternion Conjugate()const;
	Quaternion Inverse();

	Quaternion Product(const Quaternion& rhs) const;
	double Magnitude() const;

	Mat4D Matrix() const;
	Mat4D RightMatrix() const;
	Mat3D RotationMatrix() const;
	void ScaledAxis(Vector3D& w);
	Vector3D RotatedVector(const Vector3D& v) const;
	void Euler(const Vector3D& euler);
	Vector3D Euler(void) const;
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