#include "stdafx.h"
#include "Random.h"
#include <ctime>
#include <stdlib.h>

void Random::Init()
{
	srand(static_cast <unsigned> (time(0)));
}

float Random::GetFloatInRange(float low, float hi)
{
	return low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (hi - low)));
}

double Random::GetDoubleInRange(double low, double hi)
{
	return low + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (hi - low)));
}

int Random::GetIntInRange(int low, int hi)
{
	return low + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (hi - low)));
}

long Random::GetLongInRange(long low, long hi)
{
	return low + static_cast <long> (rand()) / (static_cast <long> (RAND_MAX / (hi - low)));
}

unsigned Random::GetUnsignedInRange(unsigned low, unsigned hi)
{
	return low + static_cast <unsigned> (rand()) / (static_cast <unsigned> (RAND_MAX / (hi - low)));
}
