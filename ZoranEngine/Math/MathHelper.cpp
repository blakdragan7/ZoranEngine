#include "stdafx.h"

bool IsNearlyEqual(float left, float right)
{
	return abs(left - right) < .00001;
}

bool IsNearlyEqual(long float left, long float right)
{
	return abs(left - right) < .00001;
}
