#pragma once
class Vector2D;
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

	static double lerp(double start, double end, double alpha)
	{
		return (start * alpha) + (end * 1.0 - alpha);
	}

	static int ClipRangeToLine(Vector2D outPoints[2], Vector2D inPoints[2],const Vector2D& lineNormal, double offset);

};

