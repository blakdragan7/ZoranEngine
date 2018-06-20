#include "stdafx.h"

bool IsNearlyEqual(double left, double right)
{
	return abs(left - right) < .00001;
}

bool IsNearlyEqual(float left, float right)
{
	return abs(left - right) < .00001f;
}

bool IsNearlyEqual(long double left, long double right)
{
	return abs(left - right) < .00001;
}
