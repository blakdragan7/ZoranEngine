#pragma once
#include <Core/PlatformTypes.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include "Vector3.h"
#include "Vector2.h"

#define M_PI 3.14159265359f

// TODO
// Replace this with specific typed matricies like Matrixd44 and Matrixf44 etc ..

class ZoranEngine_EXPORT Matrix44
{
private:
	float c_array[16];

	static const int array_size = 16;
	static const int rows = 4;
	static const int cols = 4;

public:
	static Matrix44 IdentityMatrix;

public:
	static Matrix44 MatrixFromArray(float* c_array);
	static Matrix44 OrthoMatrix(float left, float right, float bottom, float top, float znear = -100, float zfar = 100);
	static Matrix44 ProjectionMatrix(float fov, float ratio, float nearP, float farP);
	static Matrix44 CameraMatrix(const Vector3D& pos, const Vector3D& lookAt, const Vector3D& up);
	static Matrix44 RotationMatrix(float angle, const Vector3D& vectors);
	static Matrix44 TranslationMatrix(const Vector3D& translation);
	static Matrix44 ScaleMatrix(const Vector3D& scale);
	static Matrix44 ScaleMatrix(const Vector2D& scale);

	Matrix44(bool makeIdentity = true);
	~Matrix44(void);

	inline int getSize()const { return array_size; }
	inline int getRows()const { return rows; }
	inline int getCols()const { return cols; }

	Vector3D getScale();
	Vector3D getTransform();

	void translate(const Vector3D& trans);
	void setTranslate(const Vector3D& trans);
	void scale(const Vector3D& scale);
	void setScale(const Vector3D& scale);
	void rotate(const Vector3D& angles);

	void makeIdentity();

	Matrix44 transpose()const;
	Matrix44 inverse()const;

	void print();

	void operator =(const Matrix44& other);

	Matrix44 operator -()const;

	Matrix44 operator +(const Matrix44 &other)const;
	Matrix44 operator -(const Matrix44 &other)const;
	Matrix44 operator *(const Matrix44 &other)const;
	Vector3D operator *(const Vector3D &other)const;
	Vector2D operator *(const Vector2D &other)const;

	void operator *=(const Matrix44 &other);

	const float& operator[](int index)const;
	float& operator[](int index);

	const float& operator()(int row, int col)const;
	float& operator()(int row, int col);
};

typedef Matrix44 Mat44;
