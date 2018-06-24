#pragma once
class MathLib
{
public:
	inline static double Saturate(double val, double min, double max)
	{
		return min(max(val, min), max);
	}
};

