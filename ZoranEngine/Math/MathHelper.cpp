#include "stdafx.h"
#include <Math/Matrix44.h>

Matrix44 Matrix44::IdentityMatrix = Matrix44(true);

bool IsNearlyEqual(float left, float right)
{
	return abs(left - right) < .00001;
}
