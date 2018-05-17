#include "stdafx.h"
#include "Quaternion.h"

#include <stdexcept>

Quaternion::Quaternion()
{
	memset(vals,0,sizeof(vals));
}

Quaternion::Quaternion(const double* array)
{
	if (array == NULL)throw std::invalid_argument("Quaternion(array) array can not be null !");
	memcpy(vals, array, sizeof(vals));
}

Quaternion::Quaternion(Vector3D complex, double w)
{
	vals[0] = complex.x;
	vals[1] = complex.y;
	vals[2] = complex.z;
	vals[3] = w;
}

Quaternion::Quaternion(double x, double y, double z, double w)
{
	vals[0] = x;
	vals[1] = y;
	vals[2] = z;
	vals[3] = w;
}

Quaternion::~Quaternion()
{

}

Vector3D Quaternion::Complex()const 
{ 
	return Vector3D(vals[0], vals[1], vals[2]); 
}
Quaternion Quaternion::Conjugate()const 
{ 
	return Quaternion(-Complex(), W()); 
};
double Quaternion::Magnitude() const 
{ 
	return sqrt(vals[0] * vals[0] + vals[1] * vals[1] + vals[2] * vals[2] + vals[3] * vals[3]); 
}
Quaternion Quaternion::Inverse() 
{ 
	return Conjugate() / Magnitude(); 
}

Quaternion Quaternion::operator*(const Quaternion& rhs) const {
	return Product(rhs);
}

Quaternion Quaternion::operator*(double s) const {
	return Quaternion(Complex()*s, W()*s);
}

Quaternion operator*(double s, const Quaternion & q)
{
	return Quaternion(q.Complex()*s,q.W()*s);
}

Quaternion Quaternion::operator-(const Quaternion& rhs) const {
	return Quaternion(X() - rhs.X(), Y() - rhs.Y(), Z() - rhs.Z(), W() - rhs.W());
}

Quaternion Quaternion::operator-() const {
	return Quaternion(-X(), -Y(), -Z(), -W());
}

Quaternion Quaternion::operator/(double s) const {
	if (s == 0) throw std::invalid_argument("Quaternion operator/: Can not Devide by 0 ! ");
	return Quaternion(Complex() / s, W() / s);
}

Quaternion Quaternion::Product(const Quaternion& rhs) const {
	return Quaternion(Y()*rhs.Z() - Z()*rhs.Y() + X()*rhs.W() + W()*rhs.X(),
		Z()*rhs.X() - X()*rhs.Z() + Y()*rhs.W() + W()*rhs.Y(),
		X()*rhs.Y() - Y()*rhs.X() + Z()*rhs.W() + W()*rhs.Z(),
		W()*rhs.W() - X()*rhs.X() - Y()*rhs.Y() - Z()*rhs.Z());
}

Mat4D Quaternion::Matrix() const {
	double m[16] = {
		W(), -Z(),  Y(), X(),
		Z(),  W(), -X(), Y(),
		-Y(),  X(),  W(), Z(),
		-X(), -Y(), -Z(), W()
	};
	Mat4D mat(m);
	return mat;
}

Mat4D Quaternion::RightMatrix() const {
	double m[16] = {
		+W(), -Z(),  Y(), -X(),
		+Z(),  W(), -X(), -Y(),
		-Y(),  X(),  W(), -Z(),
		+X(),  Y(),  Z(),  W()
	};
	Mat4D mat(m);
	return mat;
}

Mat3D Quaternion::RotationMatrix() const {
	double m[9] = {
		1 - 2 * Y()*Y() - 2 * Z()*Z(), 2 * X()*Y() - 2 * Z()*W(), 2 * X()*Z() + 2 * Y()*W(),
		2 * X()*Y() + 2 * Z()*W(), 1 - 2 * X()*X() - 2 * Z()*Z(), 2 * Y()*Z() - 2 * X()*W(),
		2 * X()*Z() - 2 * Y()*W(), 2 * Y()*Z() + 2 * X()*W(), 1 - 2 * X()*X() - 2 * Y()*Y()
	};
	Mat3D mat(m);
	return mat;
}

void Quaternion::ScaledAxis(Vector3D& w) {
	double theta = w.getMagnitude();
	if (theta > 0.0001) {
		double s = sin(theta / 2.0);
		Vector3D W(w / theta * s);
		vals[0] = W.x;
		vals[1] = W.y;
		vals[2] = W.z;
		vals[3] = cos(theta / 2.0);
	}
	else {
		vals[0] = vals[1] = vals[2] = 0;
		vals[3] = 1.0;
	}
}

Vector3D Quaternion::RotatedVector(const Vector3D& v) const {
	return (((*this) * Quaternion(v, 0)) * Conjugate()).Complex();
}

void Quaternion::Euler(const Vector3D& euler) {
	double c1 = cos(euler.z * 0.5);
	double c2 = cos(euler.y * 0.5);
	double c3 = cos(euler.x * 0.5);
	double s1 = sin(euler.z * 0.5);
	double s2 = sin(euler.y * 0.5);
	double s3 = sin(euler.x * 0.5);

	vals[0] = c1*c2*s3 - s1*s2*c3;
	vals[1] = c1*s2*c3 + s1*c2*s3;
	vals[2] = s1*c2*c3 - c1*s2*s3;
	vals[3] = c1*c2*c3 + s1*s2*s3;
}

Vector3D Quaternion::Euler(void) const {
	Vector3D euler;
	const static double PI_OVER_2 = M_PI * 0.5;
	const static double EPSILON = 1e-10;
	double sqw, sqx, sqy, sqz;

	// quick conversion to Euler angles to give tilt to user
	sqw = vals[3] * vals[3];
	sqx = vals[0] * vals[0];
	sqy = vals[1] * vals[1];
	sqz = vals[2] * vals[2];

	euler.y = asin(2.0 * (vals[3] * vals[1] - vals[0] * vals[2]));
	if (PI_OVER_2 - fabs(euler.y) > EPSILON) {
		euler.z = atan2(2.0 * (vals[0] * vals[1] + vals[3] * vals[2]),
			sqx - sqy - sqz + sqw);
		euler.x = atan2(2.0 * (vals[3] * vals[0] + vals[1] * vals[2]),
			sqw - sqx - sqy + sqz);
	}
	else {
		// compute heading from local 'down' vector
		euler.z = atan2(2 * vals[1] * vals[2] - 2 * vals[0] * vals[3],
			2 * vals[0] * vals[2] + 2 * vals[1] * vals[3]);
		euler.x = 0.0;

		// If facing down, reverse yaw
		if (euler.y < 0)
			euler.z = M_PI - euler.z;
	}
	return euler;
}
// scaled linear interp
Quaternion Quaternion::Slerp(const Quaternion& q1, double t) {
	return Slerp(*this, q1, t);
}

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t) {

	double omega = acos(MathLib::Saturate(q0.vals[0] * q1.vals[0] +
		q0.vals[1] * q1.vals[1] +
		q0.vals[2] * q1.vals[2] +
		q0.vals[3] * q1.vals[3], -1, 1));
	if (fabs(omega) < 1e-10) {
		omega = 1e-10;
	}
	double som = sin(omega);
	double st0 = sin((1 - t) * omega) / som;
	double st1 = sin(t * omega) / som;

	return Quaternion(q0.vals[0] * st0 + q1.vals[0] * st1,
		q0.vals[1] * st0 + q1.vals[1] * st1,
		q0.vals[2] * st0 + q1.vals[2] * st1,
		q0.vals[3] * st0 + q1.vals[3] * st1);
}
