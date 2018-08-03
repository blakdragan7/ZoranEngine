#include "stdafx.h"

bool IsNearlyEqual(float left, float right)
{
	return abs(left - right) < .00001;
}
