#pragma once
#include "CellMath.h"
namespace cell
{
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
		Vec2(const Vec2<T>& v) { *this = v; }
		~Vec2();

		// ���������
		inline const Vec2<T>& operator = (const Vec2<T>& v)
		{
			_v[0] = v._v[0]; _v[1] = v._v[1]; return *this;
		}
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
		// ���
		inline value_type operator * (const Vec2<T>& rhs) const
		{
			return _v[0] * rhs._v[0] + _v[1] * rhs._v[1];
		}
		// ���Ա���
		inline const Vec2<T> operator * (value_type rhs) const
		{
			return Vec2<T>(_v[0] * rhs, _v[1] * rhs);
		}
		inline Vec2<T> & operator *= (value_type rhs)
		{
			_v[0] *= rhs; _v[1] *= rhs; return *this;
		}
		// ���Ա���
		inline const Vec2<T> operator / (value_type rhs) const
		{
			return Vec2<T>(_v[0] / rhs, _v[1] / rhs);
		}
		inline Vec2<T> & operator /= (value_type rhs)
		{
			_v[0] /= rhs; _v[1] /= rhs; return *this;
		}
		// �������
		inline const Vec2<T> operator + (const Vec2<T>& rhs) const
		{
			return Vec2<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1]);
		}
		inline Vec2<T> & operator += (const Vec2<T>& rhs)
		{
			_v[0] += rhs._v[0]; _v[1] += rhs._v[1]; return *this;
		}
		// �������
		inline const Vec2<T> operator - (const Vec2<T>& rhs) const
		{
			return Vec2<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1]);
		}
		inline Vec2<T> & operator -= (const Vec2<T>& rhs)
		{
			_v[0] -= rhs._v[0]; _v[1] -= rhs._v[1]; return *this;
		}
		// ����ȡ��
		inline const Vec2<T> operator - () const
		{
			return Vec2<T>(-_v[0], -_v[1]);
		}
		// ��������
		inline value_type length() const
		{
			return sqrt(_v[0] * _v[0] + _v[1] * _v[1]);
		}
		inline value_type length2(void) const
		{
			return _v[0] * _v[0] + _v[1] * _v[1];
		}
		// ������λ��
		inline const Vec2<T> normalize()
		{
			value_type norm = Vec2<T>::length();
			return Vec2<T>(_v[0]/norm, _v[1]/norm);
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

		// ��Ԫ����
		friend const Vec2<T> operator*(value_type lhs, const Vec2<T>& rhs)
		{
			return rhs*lhs;
		}
		friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& vec)
		{
			os << "(" << vec.x() << ", " << vec.y() << ")";
			return os;
		}
	};
}