#pragma once
#include "CellMath.h"
namespace cell
{
	//class Vec2f
	//{
	//public:
	//	typedef float value_type;
	//	enum { num_components = 2 };
	//public:
	//	value_type _v[num_components];
	//public:
	//	Vec2f(){ _v[0] = 0.0f, _v[1] = 0.0f; }
	//	Vec2f(value_type x, value_type y) { _v[0] = x; _v[1] = y; }
	//
	//	// 运算符重载
	//	inline bool operator == (const Vec2f& v) const { return equivalent(_v[0], v._v[0]) && equivalent(_v[1], v._v[1]); }
	//	inline bool operator != (const Vec2f& v) const { return !equivalent(_v[0], v._v[0]) && !equivalent(_v[1], v._v[1]); }
	//	inline bool operator < (const Vec2f& v) const
	//	{
	//		if (equivalent(_v[0], v._v[0]))
	//		{
	//			if (equivalent(_v[1], v._v[1])) return false;
	//			else return _v[1] < v._v[1];
	//		}
	//		else if (_v[0] < v._v[0]) return true;
	//		else return false;
	//	}
	//	inline value_type & operator [] (int i) { return _v[i]; }
	//	inline value_type operator [] (int i) const { return _v[i]; }
	//	// 点乘
	//	inline value_type operator * (const Vec2f& rhs) const
	//	{ return _v[0] * rhs._v[0] + _v[1] * rhs._v[1]; }
	//	// 乘以标量
	//	inline const Vec2f operator * (value_type rhs) const
	//	{ return Vec2f(_v[0] * rhs, _v[1] * rhs); }
	//	inline Vec2f & operator *= (value_type rhs) 
	//	{ _v[0] *= rhs; _v[1] *= rhs; return *this; }
	//	// 除以标量
	//	inline const Vec2f operator / (value_type rhs) const
	//	{ return Vec2f(_v[0] / rhs, _v[1] / rhs); }
	//	inline Vec2f & operator /= (value_type rhs)
	//	{ _v[0] /= rhs; _v[1] /= rhs; return *this; }
	//	// 向量相加
	//	inline const Vec2f operator + (const Vec2f& rhs) const
	//	{ return Vec2f(_v[0] + rhs._v[0], _v[1] + rhs._v[1]); }
	//	inline Vec2f & operator += (const Vec2f& rhs)
	//	{ _v[0] += rhs._v[0]; _v[1] += rhs._v[1]; return *this; }
	//	// 向量相减
	//	inline const Vec2f operator - (const Vec2f& rhs) const
	//	{ return Vec2f(_v[0] - rhs._v[0], _v[1] - rhs._v[1]); }
	//	inline Vec2f & operator -= (const Vec2f& rhs)
	//	{ _v[0] -= rhs._v[0]; _v[1] -= rhs._v[1]; return *this; }
	//	// 向量取反
	//	inline const Vec2f operator - () const
	//	{ return Vec2f(-_v[0], -_v[1]); }
	//	// 向量长度
	//	inline value_type length() const
	//	{ return sqrtf(_v[0] * _v[0] + _v[1] * _v[1]); }
	//	inline value_type length2(void) const
	//	{ return _v[0] * _v[0] + _v[1] * _v[1]; }
	//	// 向量单位化
	//	inline value_type normalize()
	//	{
	//		value_type norm = Vec2f::length();
	//		if (norm>0.0)
	//		{
	//			*this *= 1.0f / norm;
	//		}
	//		return(norm);
	//	}
	//
	//	// Get and Set
	//	inline value_type * ptr() { return _v; }
	//	inline const value_type * ptr() const { return _v; }
	//	inline void set(value_type x, value_type y) { _v[0] = x; _v[1] = y; }
	//	inline void set(const Vec2f& rhs) { _v[0] = rhs._v[0]; _v[1] = rhs._v[1]; }
	//	inline value_type & x() { return _v[0]; }
	//	inline value_type & y() { return _v[1]; }
	//	inline value_type x() const { return _v[0]; }
	//	inline value_type y() const { return _v[1]; }
	//
	//	inline bool valid() const { return !isNaN(); }
	//	inline bool isNaN() const { return cell::isNaN(_v[0]) || cell::isNaN(_v[1]); }
	//};

	template <typename T>
	class Vec2
	{
	public:
		typedef T value_type;
		enum { num_components = 2 };
	public:
		value_type _v[num_components];
	public:
		Vec2(){ _v[0] = value_type(0.0), _v[1] = value_type(0.0); }
		Vec2(value_type x, value_type y) { _v[0] = x; _v[1] = y; }

		// 运算符重载
		inline bool operator == (const Vec2<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1]; }
		inline bool operator != (const Vec2<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1]; }
		inline bool operator < (const Vec2<T>& v) const
		{
			if (_v[0]<v._v[0])      return true;
			else if (_v[0]>v._v[0]) return false;
			else return (_v[1]<v._v[1]);
		}
		inline value_type & operator [] (int i) { return _v[i]; }
		inline value_type operator [] (int i) const { return _v[i]; }
		// 点乘
		inline value_type operator * (const Vec2<T>& rhs) const
		{
			return _v[0] * rhs._v[0] + _v[1] * rhs._v[1];
		}
		// 乘以标量
		inline const Vec2<T> operator * (value_type rhs) const
		{
			return Vec2<T>(_v[0] * rhs, _v[1] * rhs);
		}
		inline Vec2<T> & operator *= (value_type rhs)
		{
			_v[0] *= rhs; _v[1] *= rhs; return *this;
		}
		// 除以标量
		inline const Vec2<T> operator / (value_type rhs) const
		{
			return Vec2<T>(_v[0] / rhs, _v[1] / rhs);
		}
		inline Vec2<T> & operator /= (value_type rhs)
		{
			_v[0] /= rhs; _v[1] /= rhs; return *this;
		}
		// 向量相加
		inline const Vec2<T> operator + (const Vec2<T>& rhs) const
		{
			return Vec2<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1]);
		}
		inline Vec2<T> & operator += (const Vec2<T>& rhs)
		{
			_v[0] += rhs._v[0]; _v[1] += rhs._v[1]; return *this;
		}
		// 向量相减
		inline const Vec2<T> operator - (const Vec2<T>& rhs) const
		{
			return Vec2<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1]);
		}
		inline Vec2<T> & operator -= (const Vec2<T>& rhs)
		{
			_v[0] -= rhs._v[0]; _v[1] -= rhs._v[1]; return *this;
		}
		// 向量取反
		inline const Vec2<T> operator - () const
		{
			return Vec2<T>(-_v[0], -_v[1]);
		}
		// 向量长度
		inline value_type length() const
		{
			return sqrt(_v[0] * _v[0] + _v[1] * _v[1]);
		}
		inline value_type length2(void) const
		{
			return _v[0] * _v[0] + _v[1] * _v[1];
		}
		// 向量单位化
		inline value_type normalize()
		{
			value_type norm = Vec2<T>::length();
			if (norm>0.0)
			{
				*this *= (T)1.0 / norm;
			}
			return(norm);
		}

		// Get and Set
		inline value_type * ptr() { return _v; }
		inline const value_type * ptr() const { return _v; }
		inline void set(value_type x, value_type y) { _v[0] = x; _v[1] = y; }
		inline void set(const Vec2<T>& rhs) { _v[0] = rhs._v[0]; _v[1] = rhs._v[1]; }
		inline value_type & x() { return _v[0]; }
		inline value_type & y() { return _v[1]; }
		inline value_type x() const { return _v[0]; }
		inline value_type y() const { return _v[1]; }

		inline bool valid() const { return !isNaN(); }
		inline bool isNaN() const { return cell::isNaN(_v[0]) || cell::isNaN(_v[1]); }
	};

	template <typename Vec2Type>
	Vec2Type componentMultiply(const Vec2Type& lhs, const Vec2Type& rhs)
	{ return Vec2Type(lhs[0] * rhs[0], lhs[1] * rhs[1]); }

	template <typename Vec2Type>
	Vec2Type componentDivide(const Vec2Type& lhs, const Vec2Type& rhs)
	{ return Vec2Type(lhs[0] / rhs[0], lhs[1] / rhs[1]); }
}