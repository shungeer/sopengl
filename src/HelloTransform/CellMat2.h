#pragma once
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "CellVec2.h"

namespace cell
{
	//////////////////////////////////////
	// ����洢����Ϊ����
	template <typename T>
	class Mat2
	{
	public:
		typedef T value_type;
		enum 
		{ 
			num_cell = 2,		// ÿ�л���ÿ�ж��ٸ�Ԫ��
			num_components = pow(num_cell, 2)
		};
	public:
		Mat2() { Mat2::identity(); }// ��λ����
		Mat2(const value_type src[num_components]) { Mat2::set(src); }
		Mat2(value_type m0, value_type m1, value_type m2, value_type m3) { Mat2::set(m0, m1, m2, m3); }

		// set and get
		void set(const value_type src[num_components])
		{
			m_aM[0] = src[0]; m_aM[1] = src[1]; m_aM[2] = src[2]; m_aM[3] = src[3];
		}
		void set(value_type m0, value_type m1, value_type m2, value_type m3)
		{
			m_aM[0] = m0; m_aM[1] = m1; m_aM[2] = m2; m_aM[3] = m3;
		}
		void setRow(unsigned int index, const value_type row[num_cell])
		{
			m_aM[index] = row[0]; m_aM[index + num_cell * 1] = row[1];
		}
		void setRow(unsigned int index, const Vec2<value_type>& v)
		{
			m_aM[index] = v[0]; m_aM[index + num_cell * 1] = v[1];
		}
		void setColumn(unsigned int index, const value_type col[num_cell])
		{
			m_aM[index*num_cell] = col[0];	m_aM[index*num_cell+1] = col[1];
		}
		void setColumn(unsigned int index, const Vec2<value_type>& v)
		{
			m_aM[index*num_cell] = v[0];	m_aM[index*num_cell + 1] = v[1];
		}
		const value_type* get() const
		{
			return m_aM;
		}
		value_type getDeterminant() const		// determinant �õ�����ʽ��ֵ
		{
			return m_aM[0] * m_aM[3] - m_aM[1] * m_aM[2];
		}
		///////////////////////////////////////////////////////////////////////////////
		// retrieve rotation angle in degree from rotation matrix, R
		// R = | c -s |
		//     | s  c |
		// angle = atan(s / c)
		///////////////////////////////////////////////////////////////////////////////
		float getAngle() const                       // retrieve angle (degree) from matrix
		{
			return atan2f(m_aM[1], m_aM[0]);
		}

		// ��������
		Mat2<value_type>& identity()		// ��λ����
		{
			m_aM[0] = m_aM[3] = (value_type)1;
			m_aM[1] = m_aM[2] = (value_type)0;
			return *this;
		}
		Mat2<value_type>& transpose()   // ����ת��
		{
			std::swap(m_aM[1], m_aM[2]);
			return *this;
		}
		Mat2<float> invert()		// �����(�����ڷ��ص�λ����)�����ڸ�����
		{
			value_type determinant = Mat2::getDeterminant();
			Mat2<float> inv;
			if (fabs(determinant) <= cell::EPSILON)
			{
				return inv;
			}

			float invDeterminant = 1.0f / determinant;
			inv[0] = invDeterminant * m_aM[3];
			inv[1] = -invDeterminant * m_aM[1];
			inv[2] = -invDeterminant * m_aM[2];
			inv[3] = invDeterminant * m_aM[0];
			return inv;
		}

		// operators
		Mat2<value_type> operator+(const Mat2<value_type>& rhs) const    // add rhs
		{
			return Mat2<value_type>(m_aM[0] + rhs[0], m_aM[1] + rhs[1], m_aM[2] + rhs[2], m_aM[3] + rhs[3]);
		}
		Mat2<value_type> operator-(const Mat2<value_type>& rhs) const    // subtract rhs
		{
			return Mat2<value_type>(m_aM[0] - rhs[0], m_aM[1] - rhs[1], m_aM[2] - rhs[2], m_aM[3] - rhs[3]);
		}
		Mat2<value_type>& operator+=(const Mat2<value_type>& rhs)         // add rhs and update this object
		{
			m_aM[0] += rhs[0]; m_aM[1] += rhs[1]; m_aM[2] += rhs[2]; m_aM[3] += rhs[3]; return *this;
		}
		Mat2<value_type>& operator-=(const Mat2<value_type>& rhs)         // subtract rhs and update this object
		{
			m_aM[0] -= rhs[0]; m_aM[1] -= rhs[1]; m_aM[2] -= rhs[2]; m_aM[3] -= rhs[3]; return *this;
		}
		Vec2<value_type> operator*(const Vec2<value_type>& rhs) const    // multiplication: v' = M * v
		{
			return Vec2<value_type>(m_aM[0] * rhs[0] + m_aM[2] * rhs[1], m_aM[1] * rhs[0] + m_aM[3] * rhs[1]);
		}
		Mat2<value_type> operator*(const Mat2<value_type>& rhs) const    // multiplication: M3 = M1 * M2
		{
			return Mat2<value_type>(m_aM[0] * rhs[0] + m_aM[2] * rhs[1], m_aM[1] * rhs[0] + m_aM[3] * rhs[1],
				m_aM[0] * rhs[2] + m_aM[2] * rhs[3], m_aM[1] * rhs[2] + m_aM[3] * rhs[3]);
		}
		Mat2<value_type>& operator*=(const Mat2<value_type>& rhs)         // multiplication: M1' = M1 * M2
		{
			*this = *this * rhs; return *this;
		}
		bool operator==(const Mat2<value_type>& rhs) const   // exact compare, no epsilon
		{
			return (m_aM[0] == rhs[0] && m_aM[1] == rhs[1] && m_aM[2] == rhs[2] && m_aM[3] == rhs[3]);
		}
		bool operator!=(const Mat2<value_type>& rhs) const   // exact compare, no epsilon
		{
			return (m_aM[0] != rhs[0] || m_aM[1] != rhs[1] || m_aM[2] != rhs[2] || m_aM[3] != rhs[3]);
		}
		value_type operator[](unsigned int index) const           // subscript operator v[0], v[1]
		{
			return m_aM[index];
		}
		value_type& operator[](unsigned int index)                 // subscript operator v[0], v[1]
		{
			return m_aM[index];
		}

		// friends functions
		friend Mat2<value_type> operator-(const Mat2<value_type>& rhs)                     // unary operator (-)
		{
			return Mat2<value_type>(-rhs[0], -rhs[1], -rhs[2], -rhs[3]);
		}
		friend Mat2<value_type> operator*(value_type scalar, const Mat2<value_type>& rhs)       // pre-multiplication
		{
			return Mat2<value_type>(scalar*rhs[0], scalar*rhs[1], scalar*rhs[2], scalar*rhs[3]);
		}
		friend Mat2<value_type> operator*(const Vec2<value_type>& vec, const Mat2<value_type>& rhs) // pre-multiplication
		{
			return Vec2<value_type>(vec[0]*rhs[0] + vec[1]*rhs[1], vec[0]*rhs[2] + vec[1]*rhs[3]);
		}
		friend std::ostream& operator<<(std::ostream& os, const Mat2<value_type>& m)
		{
			os << std::fixed << std::setprecision(5);
			os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[2] << "]\n"
			   << "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[3] << "]\n";
			os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
			return os;
		}

		// static functions
	private:
		value_type m_aM[num_components];
	};
}
