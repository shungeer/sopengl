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
		~Vec4();

		// ���������
		inline bool operator == (const Vec4<T>& v) const { return _v[0] == v._v[0] && _v[1] == v._v[1] && _v[2] == v._v[2] && _v[3] == v._v[3]; }
		inline bool operator != (const Vec4<T>& v) const { return _v[0] != v._v[0] || _v[1] != v._v[1] && _v[2] != v._v[2] && _v[3] != v._v[3]; }
		inline bool operator < (const Vec4<T>& v) const
		{
			if (_v[0] < v._v[0])      return true;
			else if (_v[0] > v._v[0]) return false;
			else if (_v[1] < v._v[1]) return true;
			else if (_v[1] > v._v[1]) return false;
			else return _v[2] < v._v[2];
		}
		inline value_type & operator [] (int i) { return _v[i]; }
		inline value_type operator [] (int i) const { return _v[i]; }
	};
}