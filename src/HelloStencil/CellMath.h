#pragma once
#include <cmath>

namespace cell
{
#ifdef PI
#undef PI
#undef PI_2
#endif
	const double PI = 3.14159265358979323846;
	const double PI_2 = 1.57079632679489661923;
#ifdef EPSILON
#undef EPSILON
#endif
	const double EPSILON = 1e-6;

	template<typename T>
	inline bool equivalent(T lhs, T rhs, double epsilon = EPSILON)
	{ double delta = rhs - lhs; return delta < 0.0 ? delta >= -epsilon : delta <= epsilon; }

	template<typename T>
	inline T minimum(T lhs, T rhs) { return lhs<rhs ? lhs : rhs; }
	template<typename T>
	inline T maximum(T lhs, T rhs) { return lhs>rhs ? lhs : rhs; }
	// 限制范围函数
	// 限制到某一范围之内
	template<typename T>
	inline T clampTo(T v, T minimum, T maximum) { return v<minimum ? minimum : v>maximum ? maximum : v; }
	// 限制到某一值之上
	template<typename T>
	inline T clampAbove(T v, T minimum) { return v < minimum ? minimum : v; }
	// 限制到某一范围之下
	template<typename T>
	inline T clampBelow(T v, T maximum) { return v > maximum ? maximum : v; }

	// 角度弧度函数
	inline float DegreesToRadians(float angle) { return angle*(float)PI / 180.0f; }
	inline double DegreesToRadians(double angle) { return angle*PI / 180.0; }
	inline float RadiansToDegrees(float angle) { return angle*180.0f / (float)PI; }
	inline double RadiansToDegrees(double angle) { return angle*180.0 / PI; }

	// 四舍五入
	inline float round(float v) { return v >= 0.0f ? floorf(v + 0.5f) : ceilf(v - 0.5f); }
	inline double round(double v) { return v >= 0.0 ? floor(v + 0.5) : ceil(v - 0.5); }

	// 非法判断
#if defined(_MSG_VER)
	inline bool isNaN(double v) { return _isnan(v) != 0; }
#else
	inline bool isNaN(float v) { return std::isnan(v); }
	inline bool isNaN(double v) { return std::isnan(v); }
#endif
}


