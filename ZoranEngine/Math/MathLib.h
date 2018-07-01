#pragma once
class MathLib
{
public:
	inline static double Saturate(double val, double min, double max)
	{
		return min(max(val, min), max);
	}

	template <typename T> inline constexpr
	static int signum(T x, std::false_type is_signed) 
	{
		return T(0) < x;
	}

	template <typename T> inline constexpr
	static int signum(T x, std::true_type is_signed) 
	{
		return (T(0) < x) - (x < T(0));
	}

	template <typename T> inline constexpr
	static int signum(T x) 
	{
		return signum(x, std::is_signed<T>());
	}
};

