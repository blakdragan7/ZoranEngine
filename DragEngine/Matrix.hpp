#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "Vector3D.hpp"

#define M_PI 3.14159265359f

template<typename t,unsigned int x,unsigned int y>
class Matrix
{
private:
	std::vector<t> c_array;
	int array_size;
	int rows,cols;

public:
	static Matrix<double,4,4> GLIdentityMatrix()
	{
		Matrix mat;
		mat.makeIdentity();
		return mat;
	}
	static Matrix<t,4,1> MatrixFromVector(Vector3D<t> vector)
	{
		Matrix mat;
		mat.c_array[0] = vector.x;
		mat.c_array[1] = vector.y;
		mat.c_array[2] = vector.z;
		mat.c_array[3] = 1.0;
		return mat;
	}
	static Matrix MatrixFromArray(int rows,int cols,t* c_array)
	{
		Matrix mat = Matrix(rows,cols);
		memcpy(&mat.c_array[0],c_array,sizeof(t)*mat.array_size);
		return mat;
	}
	static Matrix GLProjectionMatrix(float fov, float ratio, float nearP, float farP)
	{
		Matrix projMatrix = Matrix::GLIdentityMatrix();

		float f = 1.0f / tan (fov * (M_PI / 360.0f));
 
		projMatrix[0] = f / ratio;
		projMatrix[1 * 4 + 1] = f;
		projMatrix[2 * 4 + 2] = farP / (farP - nearP);
		projMatrix[3 * 4 + 2] = (farP * nearP) / (farP - nearP);
		projMatrix[2 * 4 + 3] = -1.0f;
		projMatrix[3 * 4 + 3] = 0.0f;

		return projMatrix;
	}
	static Matrix GLCameraMatrix(Vector3D<t> pos,Vector3D<t> lookAt,Vector3D<t> up)
	{
		Vector3D<t> dir, right;
 
		dir.x =  (lookAt.x - pos.x);
		dir.y =  (lookAt.y - pos.y);
		dir.z =  (lookAt.z - pos.z);

		dir.normalize();
 
		right = dir.cross(up);
		right.normalize();
 
		up = right.cross(dir);
		up.normalize();
 
		Matrix viewMatrix=Matrix::GLIdentityMatrix();
 
		viewMatrix[0]  = right.x;
		viewMatrix[4]  = right.y;
		viewMatrix[8]  = right.z;
		viewMatrix[12] = 0.0f;
 
		viewMatrix[1]  = up.x;
		viewMatrix[5]  = up.y;
		viewMatrix[9]  = up.z;
		viewMatrix[13] = 0.0f;
 
		viewMatrix[2]  = -dir.x;
		viewMatrix[6]  = -dir.y;
		viewMatrix[10] = -dir.z;
		viewMatrix[14] =  0.0f;
 
		viewMatrix[3]  = 0.0f;
		viewMatrix[7]  = 0.0f;
		viewMatrix[11] = 0.0f;
		viewMatrix[15] = 1.0f;

		viewMatrix = viewMatrix.translate(pos);

		return viewMatrix;
	}
	static Matrix GLRotationMatrix(float angle,Vector3D<t> vectors)
	{
		Matrix mat = Matrix(4,4);
		if(vectors.x != 0.0)
		{
			float newAngle = angle * vectors.x;
			float matVal[] = 
			{
				1.0,0.0,0.0,0.0,
				0.0,cos(newAngle),-sin(newAngle),0.0,
				0.0,sin(newAngle),cos(newAngle),0.0,
				0.0,0.0,0.0,1.0
			};
			Matrix rotX = Matrix<float>::MatrixFromArray(4,4,matVal);
			mat *= rotX;
		}
		if(vectors.y != 0.0)
		{
			float newAngle = angle * vectors.y;
			float matVal[] = 
			{
				cos(newAngle),0.0,sin(newAngle),0.0,
				0.0,1.0,0.0,0.0,
				-sin(newAngle),0.0,cos(newAngle),0.0,
				0.0,0.0,0.0,1.0
			};
			Matrix rotY = Matrix<float>::MatrixFromArray(4,4,matVal);
			mat *= rotY;
		}
		if(vectors.z != 0.0)
		{
			float newAngle = angle * vectors.z;
			float matVal[] = 
			{
				cos(newAngle),-sin(newAngle),0.0,0.0,
				sin(newAngle),cos(newAngle),0.0,0.0,
				0.0,0.0,1.0,0.0,
				0.0,0.0,0.0,1.0
			};
			Matrix rotZ = Matrix<float>::MatrixFromArray(4,4,matVal);
			mat *= rotZ;
		}
		return mat;
	}
	static Matrix GLTextureRotationMatrix(float angle)
	{
		MatrixF mat = MatrixF(2, 2);

		float matVal[] =
		{
			cos(angle),-sin(angle),
			sin(angle),cos(angle)
		};
		MatrixF rotX = Matrix<float>::MatrixFromArray(2, 2, matVal);
		mat *= rotX;
		
		return mat;
	}

	Matrix(void)
	{
		rows = x;
		cols = y;
		array_size = rows*cols;
		c_array.resize(array_size);
		makeIdentity();
	}

	Matrix(t* c_array)
	{
		rows = x;
		cols = y;
		array_size = rows*cols;
		this->c_array.resize(array_size);
		memcpy(&this->c_array[0], c_array, sizeof(t)*array_size);
	}

	~Matrix(void){}

	inline int getSize()const{return array_size;}
	inline int getRows()const{return rows;}
	inline int getCols()const{return cols;}

	Vector3D<t> getScale()
	{
		return Vector3D<t>(this->operator()(0,0), this->operator()(1, 1), this->operator()(2, 2));
	}

	Vector3D<t> getTransform()
	{
		return Vector3D<t>(this->operator()(0, 3), this->operator()(1, 3), this->operator()(2, 3));
	}

	Matrix translate(Vector3D<t> trans)
	{
		if((rows==cols) && (cols==4))
		{
			operator()(0,3) += trans.x;
			operator()(1,3) += trans.y;
			operator()(2,3) += trans.z;

			return *this;
		}
		else
			throw std::invalid_argument("Must be Perfect Matricie to Translate");
	}

	Matrix scale(Vector3D<t> scale)
	{
		if ((rows == cols) && (cols == 4))
		{
			operator()(0, 0) *= scale.x;
			operator()(1, 1) *= scale.y;
			operator()(2, 2) *= scale.z;

			return *this;
		}
		else
			throw std::invalid_argument("Must be Perfect Matricie to Translate");
	}

	Matrix rotate(float angle)
	{
		if ((cols == rows) && (rows == 2))
		{
			Matrix mat = Matrix::GLTextureRotationMatrix(angle);
			*this *= mat;
		}
		return *this;
	}

	Matrix rotate(Vec3F angles)
	{
		if((cols == rows) && (rows == 4))
		{
			if(angles.x != 0.0)
			{
				Matrix mat = Matrix::GLRotationMatrix(angles.x,Vector3D<t>(1.0,0.0,0.0));
				*this *= mat;
			}
			if(angles.y != 0.0)
			{
				Matrix mat = Matrix::GLRotationMatrix(angles.y,Vector3D<t>(0.0,1.0,0.0));
				*this *= mat;
			}
			if(angles.z != 0.0)
			{
				Matrix mat = Matrix::GLRotationMatrix(angles.z,Vector3D<t>(0.0,0.0,1.0));
				*this *= mat;
			}

			return *this;
		}
		else
			throw std::invalid_argument("cannot rotate matrix with colimns and rows != 4");
	}

	void makeIdentity()
	{
		if(rows == cols)
		{
			for(int i=0;i<rows;i++)
			{
				for(int j=0;j<cols;j++)
				{
					int index = (i * cols) + j;
					if(i==j)
						c_array[index] = 1;
					else
						c_array[index] = 0;
				}
			}
		}
		else
		{
			memset(&c_array[0],0,array_size);
		}
	}
// QT Workaround

#ifdef QT
#define transPostIfQT(a) a.transpose()
#else 
#define transPostIfQT(a) a
#endif

	Matrix transpose()
	{
		Matrix ret(cols,rows);
		for(int i=0;i<rows;i++)
		{
			for(int j=0;j<cols;j++)
			{
				ret[(j*cols) + i] = c_array[(i*rows) + j];
			}
		}
		return ret;
	}

	Matrix inverse()
	{
		if(rows == cols)
		{
			return transpose();
		}
		else
		{
			// TO DO
			return *this;
		}
	}

	void print()
	{
		std::cout << "Matrix: " << std::endl;
		for(int i=0;i<cols;i++)
		{
			std::cout << "Row: " << i << ": ";
			for(int j=0;j<rows;j++)
			{
				std::cout << c_array[(i*rows) + j] <<" ";
			}
			std::cout <<std::endl;
		}
	}

	Matrix operator +(const Matrix &other)
	{
		Matrix mat(cols,rows);
		if ( rows == other.rows && cols == other.cols )
		{
			for(int i=0;i<array_size;i++)
			{
				mat[i] += other[i];
			}
		}
		return mat;
	}

	Matrix operator -(const Matrix &other)
	{
		Matrix mat(rows,cols);
		if ( rows == other.rows && cols == other.cols )
		{
			for(int i=0;i<rows*cols;i++)
			{
				mat[i] -= other[i];
			}
		}
		return mat;
	}

	Matrix operator *(Matrix &other)
	{
		Matrix res(rows, other.cols);
		if(cols == other.rows)
		{
			for (int col = 0; col < other.cols; ++col)
			{
				for (int row = 0; row < rows; ++row) 
				{
					res(row, col) = 0;
					for (int n = 0; n < cols; n++)
						res(row, col) += ((*this)(row, n)*other(n, col));
				}
			}
		}
		else 
		{
			char var[64] = { 0 };
			snprintf(var,64,"Wrong Size Matrix Multiply First cols %d second Row %d",cols,other.rows);
			throw std::invalid_argument(var);
		}

		return res;
	}

	Matrix &operator *=(const Matrix &other)
	{
		std::vector<t> new_Array;
		new_Array.resize(array_size);
		if(rows == other.rows && cols == other.cols)
		{
			for (int i = 0; i < cols; ++i) 
			{
				for (int j = 0; j < other.rows; ++j) 
				{
					int index = (i*other.rows)+j;
					new_Array[index] = 0.0f;
					for (int k = 0; k < rows; ++k) 
					{
						int index1 = (i*rows)+k;
						int index2 = ((k*other.rows)+j);
						new_Array[index] += c_array[index1] * other.c_array[index2];
					}
				}
			}
			c_array = new_Array;
		}	
		else
			throw std::invalid_argument("Can not \"*=\" two Matricies of different sizes");
		return *this;
	}

	t& operator[](int index)
	{
		if(index >= array_size)
			throw std::exception("Index Out of Bounds For Matrix");
		return c_array[index];
	}

	t& operator()(int row,int col)
	{
		if(row >= rows)
			throw std::exception("MATRIX OUT OF BOUNDS - row is not within row bounds");
		if(col >= cols)
			throw std::exception("MATRIX OUT OF BOUNDS - col is not within column bounds");
		int index = (row*cols)+col;

		return c_array[index];
	}
};

typedef Matrix<double,4,4> Mat4D;
typedef Matrix<float,4,4>  Mat4F;
typedef Matrix<int,4,4>    Mat4I;

typedef Matrix<double, 3, 3> Mat3D;
typedef Matrix<float, 3, 3>  Mat3F;
typedef Matrix<int, 3, 3>    Mat3I;

typedef Matrix<double, 2, 2> Mat2D;
typedef Matrix<float, 2, 2>  Mat2F;
typedef Matrix<int, 2, 2>    Mat2I;
