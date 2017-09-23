#pragma once
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "CellVec3.h"

namespace cell
{
	//////////////////////////////////////
	// 矩阵存储以列为主序
	template <typename T>
	class Mat3
	{
	public:
		typedef T value_type;
		enum
		{
			num_cell = 3,		// 每行或者每列多少个元素
			num_components = num_cell * num_cell
		};
	public:
		Mat3() { Mat3::identity(); }// 单位矩阵
		Mat3(const value_type src[num_components]) { Mat3::set(src); }
		Mat3(value_type m0, value_type m1, value_type m2, 
			value_type m3, value_type m4, value_type m5,
			value_type m6, value_type m7, value_type m8) 
		{
			Mat3::set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
		}
		Mat3(const Mat3<T>& m){ *this = m; }
		~Mat3(){}

		// set and get
		void set(const value_type src[num_components])
		{
			m_aM[0] = src[0]; m_aM[1] = src[1]; m_aM[2] = src[2]; 
			m_aM[3] = src[3]; m_aM[4] = src[4]; m_aM[5] = src[5];
			m_aM[6] = src[6]; m_aM[7] = src[7]; m_aM[8] = src[8];
		}
		void set(value_type m0, value_type m1, value_type m2,
			value_type m3, value_type m4, value_type m5,
			value_type m6, value_type m7, value_type m8)
		{
			m_aM[0] = m0; m_aM[1] = m1; m_aM[2] = m2;
			m_aM[3] = m3; m_aM[4] = m4; m_aM[5] = m5;
			m_aM[6] = m6; m_aM[7] = m7; m_aM[8] = m8;
		}
		void setRow(unsigned int index, const value_type row[num_cell])
		{
			m_aM[index] = row[0]; m_aM[index + num_cell * 1] = row[1]; m_aM[index + num_cell * 2] = row[2];
		}
		void setRow(unsigned int index, const Vec3<value_type>& v)
		{
			m_aM[index] = v[0]; m_aM[index + num_cell * 1] = v[1]; m_aM[index + num_cell * 2] = v[2];
		}
		void setColumn(unsigned int index, const value_type col[num_cell])
		{
			m_aM[index*num_cell] = col[0]; m_aM[index*num_cell + 1] = col[1]; m_aM[index*num_cell + 2] = col[2];
		}
		void setColumn(unsigned int index, const Vec3<value_type>& v)
		{
			m_aM[index*num_cell] = v[0]; m_aM[index*num_cell + 1] = v[1]; m_aM[index*num_cell + 2] = v[2];
		}
		const value_type* get() const
		{
			return m_aM;
		}
		value_type getDeterminant() const		// determinant 得到行列式的值
		{
			return m_aM[0] * (m_aM[4] * m_aM[8] - m_aM[5] * m_aM[7]) -
				m_aM[1] * (m_aM[3] * m_aM[8] - m_aM[5] * m_aM[6]) +
				m_aM[2] * (m_aM[3] * m_aM[7] - m_aM[4] * m_aM[6]);
		}
		///////////////////////////////////////////////////////////////////////////////
		// retrieve angles in degree from rotation matrix, M = Rx*Ry*Rz
		// Rx: rotation about X-axis, pitch
		// Ry: rotation about Y-axis, yaw(heading)
		// Rz: rotation about Z-axis, roll
		//    Rx           Ry          Rz
		// |1  0   0| | Cy  0 Sy| |Cz -Sz 0|   | CyCz        -CySz         Sy  |
		// |0 Cx -Sx|*|  0  1  0|*|Sz  Cz 0| = | SxSyCz+CxSz -SxSySz+CxCz -SxCy|
		// |0 Sx  Cx| |-Sy  0 Cy| | 0   0 1|   |-CxSyCz+SxSz  CxSySz+SxCz  CxCy|
		//
		// Pitch: atan(-m[7] / m[8]) = atan(SxCy/CxCy)
		// Yaw  : asin(m[6]) = asin(Sy)
		// Roll : atan(-m[3] / m[0]) = atan(SzCy/CzCy)
		///////////////////////////////////////////////////////////////////////////////
		Vec3<value_type> getAngle() const                       // return (pitch, yaw, roll)
		{
			value_type pitch, yaw, roll;         // 3 angles

			// find yaw (around y-axis) first
			// NOTE: asin() returns -90~+90, so correct the angle range -180~+180
			// using z value of forward vector
			yaw = cell::RadiansToDegrees(asin(m[6]));
			if (m_aM[8] < 0)
			{
				if (yaw >= 0) yaw = (value_type)180.0 - yaw;
				else         yaw = (value_type)-180.0 - yaw;
			}

			// find roll (around z-axis) and pitch (around x-axis)
			// if forward vector is (1,0,0) or (-1,0,0), then m[0]=m[4]=m[9]=m[10]=0
			if (m_aM[0] > -EPSILON && m_aM[0] < EPSILON)
			{
				roll = 0;  //@@ assume roll=0
				pitch = cell::RadiansToDegrees(atan2(m_aM[1], m_aM[4]));
			}
			else
			{
				roll = cell::RadiansToDegrees(atan2(-m_aM[3], m_aM[0]));
				pitch = cell::RadiansToDegrees(atan2(-m_aM[7], m_aM[8]));
			}

			return Vec3<value_type>(pitch, yaw, roll);
		}

		// 矩阵性质
		Mat3<value_type>& identity()		// 单位矩阵
		{
			m_aM[0] = m_aM[4] = m_aM[8] = (value_type)1;
			m_aM[1] = m_aM[2] = m_aM[3] = m_aM[5] = m_aM[6] = m_aM[7] = (value_type)0;
			return *this;
		}
		Mat3<value_type>& transpose()   // 矩阵转置
		{
			std::swap(m_aM[1], m_aM[3]);
			std::swap(m_aM[2], m_aM[6]);
			std::swap(m_aM[5], m_aM[7]);
			return *this;
			/*return Mat3(m_aM[0], m_aM[3], m_aM[6],
				m_aM[1], m_aM[4], m_aM[7],
				m_aM[2], m_aM[5], m_aM[8]);*/
		}
		///////////////////////////////////////////////////////////////////////////////
		// inverse 3x3 matrix
		// If cannot find inverse (det=0), set identity matrix
		// M^-1 = adj(M) / det(M)
		//        | m4m8-m5m7  m7m2-m8m1  m1m5-m2m4 |
		//      = | m5m6-m3m8  m0m8-m2m6  m2m3-m0m5 | / det(M)
		//        | m3m7-m4m6  m6m1-m7m0  m0m4-m1m3 |
		///////////////////////////////////////////////////////////////////////////////
		Mat3<value_type>& invert()		// 逆矩阵(不存在返回单位矩阵)，存在浮点数
		{
			value_type determinant, invDeterminant;
			value_type tmp[9];

			tmp[0] = m_aM[4] * m_aM[8] - m_aM[5] * m_aM[7];
			tmp[1] = m_aM[7] * m_aM[2] - m_aM[8] * m_aM[1];
			tmp[2] = m_aM[1] * m_aM[5] - m_aM[2] * m_aM[4];
			tmp[3] = m_aM[5] * m_aM[6] - m_aM[3] * m_aM[8];
			tmp[4] = m_aM[0] * m_aM[8] - m_aM[2] * m_aM[6];
			tmp[5] = m_aM[2] * m_aM[3] - m_aM[0] * m_aM[5];
			tmp[6] = m_aM[3] * m_aM[7] - m_aM[4] * m_aM[6];
			tmp[7] = m_aM[6] * m_aM[1] - m_aM[7] * m_aM[0];
			tmp[8] = m_aM[0] * m_aM[4] - m_aM[1] * m_aM[3];

			// check determinant if it is 0
			determinant = m_aM[0] * tmp[0] + m_aM[1] * tmp[3] + m_aM[2] * tmp[6];
			//Mat3<value_type> inv;
			if (fabs(determinant) <= EPSILON)
			{
				return Mat3::identity(); // cannot inverse, make it idenety matrix
			}

			// divide by the determinant
			invDeterminant = (value_type)1.0 / determinant;
			m_aM[0] = invDeterminant * tmp[0];
			m_aM[1] = invDeterminant * tmp[1];
			m_aM[2] = invDeterminant * tmp[2];
			m_aM[3] = invDeterminant * tmp[3];
			m_aM[4] = invDeterminant * tmp[4];
			m_aM[5] = invDeterminant * tmp[5];
			m_aM[6] = invDeterminant * tmp[6];
			m_aM[7] = invDeterminant * tmp[7];
			m_aM[8] = invDeterminant * tmp[8];

			return *this;
		}

		// operators
		const Mat3<value_type>& operator = (const Mat3<value_type>& rhs)
		{
			Mat3::set(rhs[0], rhs[1], rhs[2], rhs[3], rhs[4], rhs[5], rhs[6], rhs[7], rhs[8]); return *this;
		}
		const Mat3<value_type> operator+(const Mat3<value_type>& rhs) const    // add rhs
		{
			return Mat3<value_type>(m_aM[0] + rhs[0], m_aM[1] + rhs[1], m_aM[2] + rhs[2], 
				m_aM[3] + rhs[3], m_aM[4] + rhs[4], m_aM[5] + rhs[5],
				m_aM[6] + rhs[6], m_aM[7] + rhs[7], m_aM[8] + rhs[8]);
		}
		const Mat3<value_type> operator-(const Mat3<value_type>& rhs) const    // subtract rhs
		{
			return Mat3<value_type>(m_aM[0] - rhs[0], m_aM[1] - rhs[1], m_aM[2] - rhs[2],
				m_aM[3] - rhs[3], m_aM[4] - rhs[4], m_aM[5] - rhs[5],
				m_aM[6] - rhs[6], m_aM[7] - rhs[7], m_aM[8] - rhs[8]);
		}
		Mat3<value_type>& operator+=(const Mat3<value_type>& rhs)         // add rhs and update this object
		{
			m_aM[0] += rhs[0]; m_aM[1] += rhs[1]; m_aM[2] += rhs[2]; 
			m_aM[3] += rhs[3]; m_aM[4] += rhs[4]; m_aM[5] += rhs[5];
			m_aM[6] += rhs[6]; m_aM[7] += rhs[7]; m_aM[8] += rhs[8];
			
			return *this;
		}
		Mat3<value_type>& operator-=(const Mat3<value_type>& rhs)         // subtract rhs and update this object
		{
			m_aM[0] -= rhs[0]; m_aM[1] -= rhs[1]; m_aM[2] -= rhs[2];
			m_aM[3] -= rhs[3]; m_aM[4] -= rhs[4]; m_aM[5] -= rhs[5];
			m_aM[6] -= rhs[6]; m_aM[7] -= rhs[7]; m_aM[8] -= rhs[8];

			return *this;
		}
	
		const Vec3<value_type> operator*(const Vec3<value_type>& rhs) const    // multiplication: v' = M * v
		{
			return Vec3<value_type>(m_aM[0] * rhs[0] + m_aM[3] * rhs[1] + m_am[6] * rhs[2],
				m_aM[1] * rhs[0] + m_aM[4] * rhs[1] + m_am[7] * rhs[2],
				m_aM[2] * rhs[0] + m_aM[5] * rhs[1] + m_am[8] * rhs[2]);
		}
		const Mat3<value_type> operator*(value_type rhs) const
		{
			return Vec3<value_type>(m_aM[0] * rhs, m_aM[1] * rhs, m_aM[2] * rhs,
				m_aM[3] * rhs, m_aM[4] * rhs, m_aM[5] * rhs,
				m_aM[6] * rhs, m_aM[7] * rhs, m_aM[8] * rhs);
		}
		const Mat3<value_type> operator*(const Mat3<value_type>& rhs) const    // multiplication: M3 = M1 * M2
		{
			return Mat3<value_type>(m_aM[0] * rhs[0] + m_aM[3] * rhs[1] + m_aM[6] * rhs[2],
				m_aM[1] * rhs[0] + m_aM[4] * rhs[1] + m_aM[7] * rhs[2],
				m_aM[2] * rhs[0] + m_aM[5] * rhs[1] + m_aM[8] * rhs[2],
				m_aM[0] * rhs[3] + m_aM[3] * rhs[4] + m_aM[6] * rhs[5],
				m_aM[1] * rhs[3] + m_aM[4] * rhs[4] + m_aM[7] * rhs[5],
				m_aM[2] * rhs[3] + m_aM[5] * rhs[4] + m_aM[8] * rhs[5], 
				m_aM[0] * rhs[6] + m_aM[3] * rhs[7] + m_aM[6] * rhs[8],
				m_aM[1] * rhs[6] + m_aM[4] * rhs[7] + m_aM[7] * rhs[8],
				m_aM[2] * rhs[6] + m_aM[5] * rhs[7] + m_aM[8] * rhs[8]);
		}
		Mat3<value_type>& operator*=(const Mat3<value_type>& rhs)         // multiplication: M1' = M1 * M2
		{
			*this = *this * rhs; return *this;
		}
		bool operator==(const Mat3<value_type>& rhs) const   // exact compare, no epsilon
		{
			return (m_aM[0] == rhs[0] && m_aM[1] == rhs[1] && m_aM[2] == rhs[2] 
				&& m_aM[3] == rhs[3] && m_aM[4] == rhs[4] && m_aM[5] == rhs[5] 
				&& m_aM[6] == rhs[6] && m_aM[7] == rhs[7] && m_aM[8] == rhs[8]);
		}
		bool operator!=(const Mat3<value_type>& rhs) const   // exact compare, no epsilon
		{
			return (m_aM[0] != rhs[0] || m_aM[1] != rhs[1] || m_aM[2] != rhs[2]
				|| m_aM[3] != rhs[3] || m_aM[4] != rhs[4] || m_aM[5] != rhs[5]
				|| m_aM[6] != rhs[6] || m_aM[7] != rhs[7] || m_aM[8] != rhs[8]);
		}
		value_type operator[](unsigned int index) const           // subscript operator v[0], v[1]
		{
			return m_aM[index];
		}
		value_type& operator[](unsigned int index)                 // subscript operator v[0], v[1]
		{
			return m_aM[index];
		}

		const Mat3<value_type> operator-()                     // unary operator (-)
		{
			return Mat3<value_type>(-m_aM[0], -m_aM[1], -m_aM[2],
				-m_aM[3], -m_aM[4], -m_aM[5],
				-m_aM[6], -m_aM[7], -m_aM[8]);
		}

		// friends functions
		template <typename T>
		friend const Mat3<T> operator*(T scalar, const Mat3<T>& rhs)       // pre-multiplication
		{
			return rhs * scalar;
		}
		template <typename T>
		friend const Vec3<T> operator*(const Vec3<T>& vec, const Mat3<T>& rhs) // pre-multiplication
		{
			return Vec3<T>(vec.x() * rhs[0] + vec.y() * rhs[1] + vec.z() * rhs[2],
				vec.x() * rhs[3] + vec.y() * rhs[4] + vec.z() * rhs[5], 
				vec.x() * rhs[6] + vec.y() * rhs[7] + vec.z() * rhs[8]);
		}
		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Mat3<T>& m)
		{
			os << std::fixed << std::setprecision(5);
			os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[3] << " " << std::setw(10) << m[6] << "]\n"
				<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[7] << "]\n"
				<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[8] << "]\n";
			os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
			return os;
		}

		// static functions
	private:
		value_type m_aM[num_components];
	};
}
