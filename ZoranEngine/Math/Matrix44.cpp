#include <stdafx.h>
#include "Matrix44.h"

Matrix44 Matrix44::MatrixFromArray(float* c_array)
{
	Matrix44 mat = Matrix44();
	memcpy(&mat.c_array[0], c_array, sizeof(float)*mat.array_size);
	return mat;
}

Matrix44 Matrix44::OrthoMatrix(float left, float right, float bottom, float top, float znear, float zfar)
{

	Matrix44 orthoMatrix;
	orthoMatrix.makeIdentity();

	orthoMatrix(0, 0) = 2.0f / (right - left);
	orthoMatrix(1, 1) = 2.0f / (top - bottom);
	orthoMatrix(2, 2) = -2.0f / (zfar - znear);
	orthoMatrix(3, 0) = -(right + left) / (right - left);
	orthoMatrix(3, 1) = -(top + bottom) / (top - bottom);
	orthoMatrix(3, 2) = -(zfar + znear) / (zfar - znear);

	return orthoMatrix;
}

Matrix44 Matrix44::ProjectionMatrix(float fov, float ratio, float nearP, float farP)
{
	Matrix44 projMatrix;

	float f = 1.0f / tan(fov * (M_PI / 360.0f));

	projMatrix[0] = f / ratio;
	projMatrix[1 * 4 + 1] = f;
	projMatrix[2 * 4 + 2] = farP / (farP - nearP);
	projMatrix[3 * 4 + 2] = (farP * nearP) / (farP - nearP);
	projMatrix[2 * 4 + 3] = -1.0f;
	projMatrix[3 * 4 + 3] = 0.0f;

	return projMatrix;
}

Matrix44 Matrix44::CameraMatrix(const Vector3D& pos, const Vector3D& lookAt, const Vector3D& up)
{
	Vector3D dir, right;

	dir.x = (lookAt.x - pos.x);
	dir.y = (lookAt.y - pos.y);
	dir.z = (lookAt.z - pos.z);

	dir.normalize();

	right = dir.cross(up);
	right.normalize();

	Vector3D right_up = right.cross(dir);
	right.normalize();

	Matrix44 viewMatrix;

	viewMatrix[0] = right.x;
	viewMatrix[4] = right.y;
	viewMatrix[8] = right.z;
	viewMatrix[12] = 0.0f;

	viewMatrix[1] = right_up.x;
	viewMatrix[5] = right_up.y;
	viewMatrix[9] = right_up.z;
	viewMatrix[13] = 0.0f;

	viewMatrix[2] = -dir.x;
	viewMatrix[6] = -dir.y;
	viewMatrix[10] = -dir.z;
	viewMatrix[14] = 0.0f;

	viewMatrix[3] = 0.0f;
	viewMatrix[7] = 0.0f;
	viewMatrix[11] = 0.0f;
	viewMatrix[15] = 1.0f;

	viewMatrix.translate(pos);

	return viewMatrix;
}

Matrix44 Matrix44::RotationMatrix(float angle, const Vector3D& vectors)
{
	Matrix44 mat;
	if (vectors.x != 0.0)
	{
		float newAngle = angle * vectors.x;
		float matVal[] =
		{
			1.0,0.0,0.0,0.0,
			0.0,cosf(newAngle),-sinf(newAngle),0.0,
			0.0,sinf(newAngle),cosf(newAngle),0.0,
			0.0,0.0,0.0,1.0
		};
		Matrix44 rotX = Matrix44::MatrixFromArray(matVal);
		mat *= rotX;
	}
	if (vectors.y != 0.0)
	{
		float newAngle = angle * vectors.y;
		float matVal[] =
		{
			cosf(newAngle),0.0,sinf(newAngle),0.0,
			0.0,1.0,0.0,0.0,
			-sinf(newAngle),0.0,cosf(newAngle),0.0,
			0.0,0.0,0.0,1.0
		};
		Matrix44 rotY = Matrix44::MatrixFromArray(matVal);
		mat *= rotY;
	}
	if (vectors.z != 0.0)
	{
		float newAngle = angle * vectors.z;
		float matVal[] =
		{
			cosf(newAngle),-sinf(newAngle),0.0,0.0,
			sinf(newAngle),cosf(newAngle),0.0,0.0,
			0.0,0.0,1.0,0.0,
			0.0,0.0,0.0,1.0
		};
		Matrix44 rotZ = Matrix44::MatrixFromArray(matVal);
		mat *= rotZ;
	}
	return mat;
}

Matrix44 Matrix44::TranslationMatrix(const Vector3D& translation)
{
	Matrix44 mat;

	mat(3, 0) = translation.x;
	mat(3, 1) = translation.y;
	mat(3, 2) = translation.z;

	return mat;
}

Matrix44 Matrix44::ScaleMatrix(const Vector3D& scale)
{
	Matrix44 mat;

	mat(0, 0) = scale.x;
	mat(1, 1) = scale.y;
	mat(2, 2) = scale.z;

	return mat;
}

Matrix44 Matrix44::ScaleMatrix(const Vector2D& scale)
{
	Matrix44 mat;

	mat(0, 0) = scale.x;
	mat(1, 1) = scale.y;
	mat(2, 2) = 1;

	return mat;
}

Matrix44::Matrix44(bool makeIdentity)
{
	if (makeIdentity)this->makeIdentity();
}

Matrix44::~Matrix44(void) {  }

Vector3D Matrix44::getScale()
{
	return Vector3D(this->operator()(0, 0), this->operator()(1, 1), this->operator()(2, 2));
}

Vector3D Matrix44::getTransform()
{
	return Vector3D(this->operator()(0, 3), this->operator()(1, 3), this->operator()(2, 3));
}

void Matrix44::translate(const Vector3D& trans)
{
	operator()(3, 0) += trans.x;
	operator()(3, 1) += trans.y;
	operator()(3, 2) += trans.z;
}


void Matrix44::setTranslate(const Vector3D& trans)
{
	operator()(3, 0) = trans.x;
	operator()(3, 1) = trans.y;
	operator()(3, 2) = trans.z;
}

void Matrix44::scale(const Vector3D& scale)
{
	operator()(0, 0) *= scale.x;
	operator()(1, 1) *= scale.y;
	operator()(2, 2) *= scale.z;
}

void Matrix44::setScale(const Vector3D& scale)
{
	operator()(0, 0) = scale.x;
	operator()(1, 1) = scale.y;
	operator()(2, 2) = scale.z;
}

void Matrix44::rotate(const Vector3D& angles)
{
	if (angles.x != 0.0)
	{
		Matrix44 mat = Matrix44::RotationMatrix(angles.x, Vector3D(1.0, 0.0, 0.0));
		*this *= mat;
	}
	if (angles.y != 0.0)
	{
		Matrix44 mat = Matrix44::RotationMatrix(angles.y, Vector3D(0.0, 1.0, 0.0));
		*this *= mat;
	}
	if (angles.z != 0.0)
	{
		Matrix44 mat = Matrix44::RotationMatrix(angles.z, Vector3D(0.0, 0.0, 1.0));
		*this *= mat;
	}

}

void Matrix44::makeIdentity()
{
	memset(c_array, 0, sizeof(c_array));
	c_array[0] = 1;
	c_array[5] = 1;
	c_array[10] = 1;
	c_array[15] = 1;
}

Matrix44 Matrix44::transpose()const
{
	Matrix44 ret;
	for (int i = 0; i<rows; i++)
	{
		for (int j = 0; j<cols; j++)
		{
			ret[(j*cols) + i] = c_array[(i*rows) + j];
		}
	}
	return ret;
}

Matrix44 Matrix44::inverse()const
{
	return transpose();
}

void Matrix44::print()
{
	for (int i = 0; i<cols; i++)
	{
		for (int j = 0; j<rows; j++)
		{
			std::cout << this->operator()(i,j) << " ";
		}
		std::cout << std::endl;
	}
}

void Matrix44::operator =(const Matrix44& other)
{
	memcpy(c_array, other.c_array, array_size * sizeof(float));
}

Matrix44 Matrix44::operator +(const Matrix44 &other)const
{
	Matrix44 mat;
	for (int i = 0; i<array_size; i++)
	{
		mat[i] += other[i];
	}
	return mat;
}

Matrix44 Matrix44::operator -(const Matrix44 &other)const
{
	Matrix44 mat;
	for (int i = 0; i<rows*cols; i++)
	{
		mat[i] -= other[i];
	}
	return mat;
}

Matrix44 Matrix44::operator *(const Matrix44 &other)const
{
	Matrix44 res;

	for (int col = 0; col < cols; ++col)
	{
		for (int row = 0; row < rows; ++row)
		{
			res(col, row) = 0;
			for (int n = 0; n < cols; n++)
				res(col, row) += ((*this)(n, row)*other(col, n));
		}
	}

	return res;
}

Vector3D Matrix44::operator *(const Vector3D &other)const
{
	float res[4];

	const Matrix44 &local = *this;

	constexpr static const float w = 1;

	res[0] = other.x*local[0] + other.y * local[1] + other.z * local[2] + w * local[3];
	res[1] = other.x*local[4] + other.y * local[5] + other.z * local[6] + w * local[7];
	res[2] = other.x*local[8] + other.y * local[9] + other.z * local[10] + w * local[11];
	res[3] = other.x*local[12] + other.y * local[13] + other.z * local[14] + w * local[0];

	res[0] /= res[3];
	res[1] /= res[3];
	res[2] /= res[3];

	return Vector3D(res[0], res[1], res[2]);
}

Vector2D Matrix44::operator *(const Vector2D &other)const
{
	float res[4];

	const Matrix44 &local = *this;

	constexpr static const float z = 1;
	constexpr static const float w = 1;

	res[0] = other.x*local[0] + other.y * local[1] + z * local[2] + w * local[3];
	res[1] = other.x*local[4] + other.y * local[5] + z * local[6] + w * local[7];
	res[2] = other.x*local[8] + other.y * local[9] + z * local[10] + w * local[11];
	res[3] = other.x*local[12] + other.y * local[13] + z * local[14] + w * local[0];

	res[0] /= res[3];
	res[1] /= res[3];
	res[2] /= res[3];

	return Vector2D(res[0], res[1]);
}

void Matrix44::operator *=(const Matrix44 &other)
{
	float new_Array[16];

	for (int i = 0; i < cols; ++i)
	{
		for (int j = 0; j < other.rows; ++j)
		{
			int index = (i*other.rows) + j;
			new_Array[index] = 0.0f;
			for (int k = 0; k < rows; ++k)
			{
				int index1 = (i*rows) + k;
				int index2 = ((k*other.rows) + j);
				new_Array[index] += c_array[index1] * other.c_array[index2];
			}
		}
	}
	memcpy(c_array, new_Array, sizeof(new_Array));
}

const float& Matrix44::operator[](int index)const
{
	assert(index < array_size && "Index Greater Then Possible Matrix 4x4 Value !!");
	return c_array[index];
}

float& Matrix44::operator[](int index)
{
	assert(index < array_size && "Index Greater Then Possible Matrix 4x4 Value !!");
	return c_array[index];
}

const float& Matrix44::operator()(int row, int col)const
{
	assert(row < rows && col < cols && "row or col is greater then 4 in Mat44 operator() const!");
	int index = (col*rows) + row;

	return c_array[index];
}

float& Matrix44::operator()(int row, int col)
{
	assert(row < rows && col < cols && "row or col is greater then 4 in Mat44 operator() !");
	int index = (col*rows) + row;

	return c_array[index];
}