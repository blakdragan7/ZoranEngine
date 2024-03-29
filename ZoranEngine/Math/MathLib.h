#pragma once
class Vector2D;
class Matrix22;
struct ClipedVertex;

class MathLib
{
public:
	inline static float Saturate(float val, float min, float max)
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

	template<class t>
	static t lerp(t start, t end, float alpha)
	{
		return (start * (1.0f - alpha)) + (end * alpha);
	}

	static int ClipRangeToLine(ClipedVertex outPoints[2], ClipedVertex inPoints[2], const Vector2D& lineNormal, float offset, char clipEdge);
	static void ComputeIncidentEdge(ClipedVertex c[2], const Vector2D& h, const Vector2D& pos,const Matrix22& Rot, const Vector2D& normal);
	static float Clamp(float value, float minv, float max);
	static Vector2D MathLib::Clamp(Vector2D value, Vector2D minv, Vector2D maxv);
};

