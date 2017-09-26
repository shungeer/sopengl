#pragma once
#include "CellMath.h"
#include "CellVec3.h"
namespace cell
{
	template <typename T>
	class Vec4
	{
	public:
		typedef T value_type;
		enum { num_components = 4 };
	public:
		value_type _v[num_components];
	public:
		Vec4(){ _v[0] = value_type(0.0), _v[1] = value_type(0.0); _v[2] = value_type(0.0); _v[3] = value_type(0.0); }
		Vec4(value_type x, value_type y, value_type z, value_type w) { _v[0] = x; _v[1] = y; _v[2] = z; _v[3] = w; }
		Vec4(const Vec3<T>& v3, value_type w) { _v[0] = v3[0]; _v[1] = v3[1]; _v[2] = v3[2]; _v[3] = w; }
		Vec4(const Vec4<T>& v){ *this = v; }
		~Vec4(){}

		// 运算符重载
		inline const Vec4<T>& operator = (const Vec4<T>& v)
		{
			_v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; _v[3] = v._v[3]; return *this;
		}
		inline bool operator == (const Vec4<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3]; }
		inline bool operator != (const Vec4<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2] || _v[3] != v._v[3]; }
		inline bool operator < (const Vec4<T>& v) const
		{
			if (_v[0] < v._v[0])      return true;
			else if (_v[0] > v._v[0]) return false;
			else if (_v[1] < v._v[1]) return true;
			else if (_v[1] > v._v[1]) return false;
			else if (_v[2] < v._v[2]) return true;
			else if (_v[2] > v._v[2]) return false;
			else return (_v[3] < v._v[3]);
		}
		inline value_type & operator [] (int i) { return _v[i]; }
		inline value_type operator [] (int i) const { return _v[i]; }
		// 点乘
		inline value_type operator * (const Vec4<T>& rhs) const
		{
			return _v[0] * rhs._v[0] + _v[1] * rhs._v[1] + _v[2] * rhs._v[2] + _v[3] * rhs._v[3];
		}
		// 乘以标量
		inline const Vec4<T> operator * (value_type rhs) const
		{
			return Vec4<T>(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
		}
		inline Vec4<T> & operator *= (value_type rhs)
		{
			_v[0] *= rhs; _v[1] *= rhs; _v[2] *= rhs; _v[3] *= rhs; return *this;
		}
		// 除以标量
		inline const Vec4<T> operator / (value_type rhs) const
		{
			return Vec4<T>(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs, _v[3] / rhs);
		}
		inline Vec4<T> & operator /= (value_type rhs)
		{
			_v[0] /= rhs; _v[1] /= rhs; _v[2] /= rhs; _v[3] /= rhs; return *this;
		}
		// 向量相加
		inline const Vec4<T> operator + (const Vec4<T>& rhs) const
		{
			return Vec4<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2], _v[3] + rhs._v[3]);
		}
		inline Vec4<T> & operator += (const Vec4<T>& rhs)
		{
			_v[0] += rhs._v[0]; _v[1] += rhs._v[1]; _v[2] += rhs._v[2]; _v[3] += rhs._v[3]; return *this;
		}
		// 向量相减
		inline const Vec4<T> operator - (const Vec4<T>& rhs) const
		{
			return Vec4<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2], _v[3] - rhs._v[3]);
		}
		inline Vec4<T> & operator -= (const Vec4<T>& rhs)
		{
			_v[0] -= rhs._v[0]; _v[1] -= rhs._v[1]; _v[2] -= rhs._v[2]; _v[3] -= rhs._v[3]; return *this;
		}
		// 向量取反
		inline const Vec4<T> operator - () const
		{
			return Vec4<T>(-_v[0], -_v[1], -_v[2], -_v[3]);
		}
		// 向量长度
		inline value_type length() const
		{
			return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
		}
		inline value_type length2(void) const
		{
			return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
		}
		// 向量单位化
		inline Vec4<T>& normalize()
		{
			value_type norm = (T)1.0 / Vec4<T>::length();
			*this *= norm;
			return *this;
		}

		// Get and Set
		inline value_type * ptr() { return _v; }
		inline const value_type * ptr() const { return _v; }
		inline void set(value_type x, value_type y, value_type z, value_type w) { _v[0] = x; _v[1] = y; _v[2] = z; _v[3] = w; }
		inline void set(const Vec4<T>& rhs) { _v[0] = rhs._v[0]; _v[1] = rhs._v[1]; _v[2] = rhs._v[2]; _v[3] = rhs._v[3]; }
		inline value_type & x() { return _v[0]; }
		inline value_type & y() { return _v[1]; }
		inline value_type & z() { return _v[2]; }
		inline value_type & w() { return _v[3]; }
		inline value_type x() const { return _v[0]; }
		inline value_type y() const { return _v[1]; }
		inline value_type z() const { return _v[2]; }
		inline value_type w() const { return _v[3]; }

		// 友元函数
		template <typename T>
		friend const Vec4<T> operator*(const T lhs, const Vec4<T>& rhs)
		{
			return rhs * lhs;
		}
		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Vec4<T>& vec)
		{
			os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ", " << vec.w() << ")";
			return os;
		}
	};
}