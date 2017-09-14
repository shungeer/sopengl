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
			num_components = pow(num_cell, 2)
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
		Vec3<float> getAngle() const                       // return (pitch, yaw, roll)
		{
			float pitch, yaw, roll;         // 3 angles

			// find yaw (around y-axis) first
			// NOTE: asin() returns -90~+90, so correct the angle range -180~+180
			// using z value of forward vector
			yaw = RAD2DEG * asinf(m[6]);
			if (m[8] < 0)
			{
				if (yaw >= 0) yaw = 180.0f - yaw;
				else         yaw = -180.0f - yaw;
			}

			// find roll (around z-axis) and pitch (around x-axis)
			// if forward vector is (1,0,0) or (-1,0,0), then m[0]=m[4]=m[9]=m[10]=0
			if (m[0] > -EPSILON && m[0] < EPSILON)
			{
				roll = 0;  //@@ assume roll=0
				pitch = RAD2DEG * atan2f(m[1], m[4]);
			}
			else
			{
				roll = RAD2DEG * atan2f(-m[3], m[0]);
				pitch = RAD2DEG * atan2f(-m[7], m[8]);
			}

			return Vec3<float>(pitch, yaw, roll);
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
		}
		///////////////////////////////////////////////////////////////////////////////
		// inverse 3x3 matrix
		// If cannot find inverse (det=0), set identity matrix
		// M^-1 = adj(M) / det(M)
		//        | m4m8-m5m7  m7m2-m8m1  m1m5-m2m4 |
		//      = | m5m6-m3m8  m0m8-m2m6  m2m3-m0m5 | / det(M)
		//        | m3m7-m4m6  m6m1-m7m0  m0m4-m1m3 |
		///////////////////////////////////////////////////////////////////////////////
		Mat3<float> invert()		// 逆矩阵(不存在返回单位矩阵)，存在浮点数
		{
			float determinant, invDeterminant;
			float tmp[9];

			tmp[0] = m_aM[4] * m_aM[8] - m_aM[5] * m_aM[7];
			tmp[1] = m_aM[5] * m_aM[6] - m_aM[3] * m_aM[8];
			tmp[2] = m_aM[3] * m_aM[7] - m_aM[4] * m_aM[6];
			tmp[3] = m_aM[7] * m_aM[2] - m_aM[8] * m_aM[1];
			tmp[4] = m_aM[0] * m_aM[8] - m_aM[2] * m_aM[6];
			tmp[5] = m_aM[6] * m_aM[1] - m_aM[7] * m_aM[0];
			tmp[6] = m_aM[1] * m_aM[5] - m_aM[2] * m_aM[4];
			tmp[7] = m_aM[2] * m_aM[3] - m_aM[0] * m_aM[5];
			tmp[8] = m_aM[0] * m_aM[4] - m_aM[1] * m_aM[3];

			// check determinant if it is 0
			determinant = m_aM[0] * tmp[0] + m_aM[1] * tmp[3] + m_aM[2] * tmp[6];
			Mat3<float> inv;
			if (fabs(determinant) <= EPSILON)
			{
				return inv; // cannot inverse, make it idenety matrix
			}

			// divide by the determinant
			invDeterminant = 1.0f / determinant;
			inv[0] = invDeterminant * tmp[0];
			inv[1] = invDeterminant * tmp[1];
			inv[2] = invDeterminant * tmp[2];
			inv[3] = invDeterminant * tmp[3];
			inv[4] = invDeterminant * tmp[4];
			inv[5] = invDeterminant * tmp[5];
			inv[6] = invDeterminant * tmp[6];
			inv[7] = invDeterminant * tmp[7];
			inv[8] = invDeterminant * tmp[8];

			return inv;
		}

		// operators
		Mat3<value_type> operator+(const Mat3<value_type>& rhs) const    // add rhs
		{
			return Mat3<value_type>(m_aM[0] + rhs[0], m_aM[1] + rhs[1], m_aM[2] + rhs[2], 
				m_aM[3] + rhs[3], m_aM[4] + rhs[4], m_aM[5] + rhs[5],
				m_aM[6] + rhs[6], m_aM[7] + rhs[7], m_aM[8] + rhs[8]);
		}
		Mat3<value_type> operator-(const Mat3<value_type>& rhs) const    // subtract rhs
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
		Vec3<value_type> operator*(const Vec3<value_type>& rhs) const    // multiplication: v' = M * v
		{
			return Vec3<value_type>(m_aM[0] * rhs[0] + m_aM[3] * rhs[1] + m_am[6] * rhs[2],
				m_aM[1] * rhs[0] + m_aM[4] * rhs[1] + m_am[7] * rhs[2],
				m_aM[2] * rhs[0] + m_aM[5] * rhs[1] + m_am[8] * rhs[2]);
		}
		Mat3<value_type> operator*(const Mat3<value_type>& rhs) const    // multiplication: M3 = M1 * M2
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

		// friends functions
		friend Mat3<value_type> operator-(const Mat3<value_type>& rhs)                     // unary operator (-)
		{
			return Mat3<value_type>(-rhs[0], -rhs[1], -rhs[2], 
				-rhs[3], -rhs[4], -rhs[5], 
				-rhs[6], -rhs[7], -rhs[8]);
		}
		friend Mat3<value_type> operator*(value_type scalar, const Mat3<value_type>& rhs)       // pre-multiplication
		{
			return Mat3<value_type>(scalar*rhs[0], scalar*rhs[1], scalar*rhs[2], 
				scalar*rhs[3], scalar*rhs[4], scalar*rhs[5], 
				scalar*rhs[6], scalar*rhs[7], scalar*rhs[8]);
		}
		friend Mat3<value_type> operator*(const Vec3<value_type>& vec, const Mat3<value_type>& rhs) // pre-multiplication
		{
			return Vec3<value_type>(vec[0] * rhs[0] + vec[1] * rhs[1] + vec[2] * rhs[2], 
				vec[0] * rhs[3] + vec[1] * rhs[4] + vec[2] * rhs[5],
				vec[0] * rhs[6] + vec[1] * rhs[7] + vec[2] * rhs[8]);
		}
		friend std::ostream& operator<<(std::ostream& os, const Mat3<value_type>& m)
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
