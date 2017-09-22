#pragma once
#include "CellMath.h"
#include "CellVec2.h"
namespace cell
{
	template <typename T>
	class Vec3
	{
	public:
		typedef T value_type;
		enum { num_components = 3 };
	public:
		value_type _v[num_components];
	public:
		Vec3(){ _v[0] = value_type(0.0), _v[1] = value_type(0.0); _v[2] = value_type(0.0); }
		Vec3(value_type x, value_type y, value_type z) { _v[0] = x; _v[1] = y; _v[2] = z; }
		Vec3(const Vec2<T>& v2, value_type zz) { _v[0] = v2[0]; _v[1] = v2[1]; _v[2] = zz; }
		Vec3(const Vec3<T>& v) { *this = v; }
		~Vec3();

		// ���������
		inline const Vec3<T>& operator = (const Vec3<T>& v)
		{
			_v[0] = v._v[0]; _v[1] = v._v[1]; _v[2] = v._v[2]; return *this;
		}
		inline bool operator == (const Vec3<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2]; }
		inline bool operator != (const Vec3<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] || _v[2] != v._v[2]; }
		inline bool operator < (const Vec3<T>& v) const
		{
			if (_v[0] < v._v[0])      return true;
			else if (_v[0] > v._v[0]) return false;
			else if (_v[1] < v._v[1]) return true;
			else if (_v[1] > v._v[1]) return false;
			else return _v[2] < v._v[2];
		}
		inline value_type & operator [] (int i) { return _v[i]; }
		inline value_type operator [] (int i) const { return _v[i]; }
		// ���
		inline value_type dot(const Vec3<T>& rhs) const
		{
			return *this * rhs;
		}
		inline value_type operator * (const Vec3<T>& rhs) const
		{
			return _v[0] * rhs._v[0] + _v[1] * rhs._v[1] + _v[2] * rhs._v[2];
		}
		// ���
		inline const Vec3<T> cross(const Vec3<T>& rhs) const
		{
			return *this ^ rhs;
		}
		inline const Vec3<T> operator ^ (const Vec3<T>& rhs) const
		{
			return Vec3f(_v[1] * rhs._v[2] - _v[2] * rhs._v[1],
				_v[2] * rhs._v[0] - _v[0] * rhs._v[2],
				_v[0] * rhs._v[1] - _v[1] * rhs._v[0]);
		}
		// ���Ա���
		inline const Vec3<T> operator * (value_type rhs) const
		{
			return Vec3<T>(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs);
		}
		inline Vec3<T> & operator *= (value_type rhs)
		{
			_v[0] *= rhs; _v[1] *= rhs; _v[2] *= rhs; return *this;
		}
		// ���Ա���
		inline const Vec3<T> operator / (value_type rhs) const
		{
			return Vec3<T>(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs);
		}
		inline Vec3<T> & operator /= (value_type rhs)
		{
			_v[0] /= rhs; _v[1] /= rhs; _v[2] /= rhs; return *this;
		}
		// �������
		inline const Vec3<T> operator + (const Vec3<T>& rhs) const
		{
			return Vec3<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2]);
		}
		inline Vec3<T> & operator += (const Vec3<T>& rhs)
		{
			_v[0] += rhs._v[0]; _v[1] += rhs._v[1]; _v[2] += rhs._v[2]; return *this;
		}
		// �������
		inline const Vec3<T> operator - (const Vec3<T>& rhs) const
		{
			return Vec3<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2]);
		}
		inline Vec3<T> & operator -= (const Vec3<T>& rhs)
		{
			_v[0] -= rhs._v[0]; _v[1] -= rhs._v[1]; _v[2] -= rhs._v[2]; return *this;
		}
		// ����ȡ��
		inline const Vec3<T> operator - () const
		{
			return Vec3<T>(-_v[0], -_v[1], -_v[2]);
		}
		// ��������
		inline value_type length() const
		{
			return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
		}
		inline value_type length2(void) const
		{
			return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2];
		}
		// ������λ��
		inline const Vec3<T> normalize()
		{
			value_type norm = Vec3<T>::length();
			return Vec3<T>(_v[0]/norm, _v[1]/norm, _v[2]/norm);
		}

		// Get and Set
		inline value_type * ptr() { return _v; }
		inline const value_type * ptr() const { return _v; }
		inline void set(value_type x, value_type y, value_type z) { _v[0] = x; _v[1] = y; _v[2] = z; }
		inline void set(const Vec3<T>& rhs) { _v[0] = rhs._v[0]; _v[1] = rhs._v[1]; _v[2] = rhs._v[2]; }
		inline value_type & x() { return _v[0]; }
		inline value_type & y() { return _v[1]; }
		inline value_type & z() { return _v[2]; }
		inline value_type x() const { return _v[0]; }
		inline value_type y() const { return _v[1]; }
		inline value_type z() const { return _v[2]; }

		inline bool valid() const { return !isNaN(); }
		inline bool isNaN() const { return cell::isNaN(_v[0]) || cell::isNaN(_v[1]) || cell::isNaN(_v[2]); }

		// ��Ԫ����
		friend const Vec3<T> operator*(const value_type lhs, const Vec3<T>& rhs)
		{
			return rhs * lhs;
		}
		friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& vec)
		{
			os << "(" << vec.x() << ", " << vec.y() << ", " << vec.z() << ")";
			return os;
		}
	};
}