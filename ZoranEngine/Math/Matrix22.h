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
	Matrix22(double rotation)
	{
		double c = cos(rotation);
		double s = sin(rotation);

		cols[0].x = c; 
		cols[0].y = -s;
		cols[1].x = s; 
		cols[1].y = c;

	}
	Matrix22(double a, double b, double c, double d)
	{
		cols[0].x = a;
		cols[0].y = b;
		cols[1].x = c;
		cols[1].y = d;
	}

	void Set(double a, double b, double c, double d)
	{
		cols[0].x = a;
		cols[0].y = b;
		cols[1].x = c;
		cols[1].y = d;
	}

	void SetRotation(double rotation)
	{
		double c = cos(rotation);
		double s = sin(rotation);

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
		
		double a = cols[0].x;
		double b = cols[1].x;
		double c = cols[0].y;
		double d = cols[1].y;

		double det = a * d - b * c;

		assert(det != 0.0f);
		
		det = 1.0 / det;
		ret.cols[0].x = det * d;	
		ret.cols[1].x = -det * b;
		ret.cols[0].y = -det * c;	
		ret.cols[1].y = det * a;

		return ret;
	}
};
