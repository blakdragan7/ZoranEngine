#pragma once
#include <Math/Vector2.h>
#include <math.h>
#include <assert.h>

class Matrix22
{
private:
		Vector2D cols[2];

public:
	Matrix22() {}
	~Matrix22() {}
	Matrix22(float rotation)
	{
		float c = cos(rotation);
		float s = sin(rotation);

		cols[0].x = c; 
		cols[0].y = -s;
		cols[1].x = s; 
		cols[1].y = c;

	}
	Matrix22(float a, float b, float c, float d)
	{
		cols[0].x = a;
		cols[0].y = b;
		cols[1].x = c;
		cols[1].y = d;
	}

	Matrix22(Vector2D col1, Vector2D col2)
	{
		cols[0] = col1;
		cols[1] = col2;
	}

	void Set(float a, float b, float c, float d)
	{
		cols[0].x = a;
		cols[0].y = b;
		cols[1].x = c;
		cols[1].y = d;
	}

	void Set(Vector2D col1, Vector2D col2)
	{
		cols[0] = col1;
		cols[1] = col2;
	}

	void SetRotation(float rotation)
	{
		float c = cos(rotation);
		float s = sin(rotation);

		cols[0].x = c;
		cols[0].y = -s;
		cols[1].x = s;
		cols[1].y = c;
	}

	Matrix22 GetTranspose()const
	{
		return Matrix22(cols[0].x, cols[1].x, cols[0].y, cols[1].y);
	}

	Matrix22 GetInversion()const
	{
		Matrix22 ret;
		
		float a = cols[0].x;
		float b = cols[1].x;
		float c = cols[0].y;
		float d = cols[1].y;

		float det = a * d - b * c;

		assert(det != 0.0f);
		
		det = 1.0 / det;
		ret.cols[0].x = det * d;	
		ret.cols[1].x = -det * b;
		ret.cols[0].y = -det * c;	
		ret.cols[1].y = det * a;

		return ret;
	}

	Matrix22 GetAbs()const
	{
		return Matrix22(cols[0].getAbs(),cols[1].getAbs());
	}

	inline Vector2D operator * (const Vector2D& v)const
	{
		return Vector2D(cols[0].x * v.x + cols[1].x * v.y, cols[0].y * v.x + cols[1].y * v.y);
	}

	inline Matrix22 operator + (const Matrix22& B)const
	{
		return Matrix22(cols[0] + B.cols[0], cols[1] + B.cols[1]);
	}

	inline Matrix22 operator - (const Matrix22& B)const
	{
		return Matrix22(cols[0] - B.cols[0], cols[1] - B.cols[1]);
	}

	inline Matrix22 operator * (const Matrix22& B)const
	{
		return Matrix22(*this * B.cols[0], *this * B.cols[1]);
	}

	inline void operator += (const Matrix22& B)
	{
		cols[0] += B.cols[0];
		cols[1] += B.cols[1];
	}

	inline void operator -= (const Matrix22& B)
	{
		cols[0] -= B.cols[0];
		cols[1] -= B.cols[1];
	}
};


