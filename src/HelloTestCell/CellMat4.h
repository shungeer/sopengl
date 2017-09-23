#pragma once
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "CellVec4.h"
#include "CellMat2.h"
#include "CellMat3.h"

namespace cell
{
	//////////////////////////////////////
	// ����洢����Ϊ����
	template <typename T>
	class Mat4
	{
	public:
		typedef T value_type;
		enum
		{
			num_cell = 4,		// ÿ�л���ÿ�ж��ٸ�Ԫ��
			num_components = pow(num_cell, 2)
		};
	public:
		Mat4() { Mat4::identity(); }// ��λ����
		Mat4(const value_type src[num_components]) { Mat4::set(src); }
		Mat4(value_type m0, value_type m1, value_type m2, value_type m3,
			value_type m4, value_type m5, value_type m6, value_type m7,
			value_type m8, value_type m9, value_type m10, value_type m11,
			value_type m12, value_type m13, value_type m14, value_type m15)
		{
			Mat4::set(m0, m1, m2, m3, m4, m5, m6, m7, m8, m9, m10, m11, m12, m13, m14, m15);
		}
		Mat4(const Mat4<T>& m){ *this = m; }
		~Mat4();

		// set and get
		void set(const value_type src[num_components])
		{
			m_aM[0] = src[0]; m_aM[1] = src[1]; m_aM[2] = src[2]; m_aM[3] = src[3];
			m_aM[4] = src[4]; m_aM[5] = src[5]; m_aM[6] = src[6]; m_aM[7] = src[7];
			m_aM[8] = src[8]; m_aM[9] = src[9]; m_aM[10] = src[10]; m_aM[11] = src[11];
			m_aM[12] = src[12]; m_aM[13] = src[13]; m_aM[14] = src[14]; m_aM[15] = src[15];
		}
		void set(value_type m0, value_type m1, value_type m2, value_type m3,
			value_type m4, value_type m5, value_type m6, value_type m7,
			value_type m8, value_type m9, value_type m10, value_type m11,
			value_type m12, value_type m13, value_type m14, value_type m15)
		{
			m_aM[0] = src[0]; m_aM[1] = src[1]; m_aM[2] = src[2]; m_aM[3] = src[3];
			m_aM[4] = src[4]; m_aM[5] = src[5]; m_aM[6] = src[6]; m_aM[7] = src[7];
			m_aM[8] = src[8]; m_aM[9] = src[9]; m_aM[10] = src[10]; m_aM[11] = src[11];
			m_aM[12] = src[12]; m_aM[13] = src[13]; m_aM[14] = src[14]; m_aM[15] = src[15];
		}
		void setRow(unsigned int index, const value_type row[num_cell])
		{
			m_aM[index] = row[0]; m_aM[index + num_cell * 1] = row[1]; 
			m_aM[index + num_cell * 2] = row[2]; m_aM[index + num_cell * 3] = row[3];
		}
		void setRow(unsigned int index, const Vec4<value_type>& v)
		{
			m_aM[index] = v[0]; m_aM[index + num_cell * 1] = v[1]; 
			m_aM[index + num_cell * 2] = v[2]; m_aM[index + num_cell * 3] = v[3];
		}
		void setColumn(unsigned int index, const value_type col[num_cell])
		{
			m_aM[index*num_cell] = col[0]; m_aM[index*num_cell + 1] = col[1]; 
			m_aM[index*num_cell + 2] = col[2]; m_aM[index*num_cell + 3] = col[3];
		}
		void setColumn(unsigned int index, const Vec4<value_type>& v)
		{
			m_aM[index*num_cell] = v[0]; m_aM[index*num_cell + 1] = v[1]; 
			m_aM[index*num_cell + 2] = v[2]; m_aM[index*num_cell + 3] = v[3];
		}
		const value_type* get() const
		{
			return m_aM;
		}
		value_type getDeterminant() const		// determinant �õ�����ʽ��ֵ
		{
			return m_aM[0] * getCofactor(m_aM[5], m_aM[6], m_aM[7], m_aM[9], m_aM[10], m_aM[11], m_aM[13], m_aM[14], m_aM[15]) -
				m_aM[1] * getCofactor(m_aM[4], m_aM[6], m_aM[7], m_aM[8], m_aM[10], m_aM[11], m_aM[12], m_aM[14], m_aM[15]) +
				m_aM[2] * getCofactor(m_aM[4], m_aM[5], m_aM[7], m_aM[8], m_aM[9], m_aM[11], m_aM[12], m_aM[13], m_aM[15]) -
				m_aM[3] * getCofactor(m_aM[4], m_aM[5], m_aM[6], m_aM[8], m_aM[9], m_aM[10], m_aM[12], m_aM[13], m_aM[14]);
		}

		// 3x3����ʽ
		value_type getCofactor(value_type m0, value_type m1, value_type m2,
			value_type m3, value_type m4, value_type m5,
			value_type m6, value_type m7, value_type m8) const
		{
			return m0 * (m4 * m8 - m5 * m7) -
				m1 * (m3 * m8 - m5 * m6) +
				m2 * (m3 * m7 - m4 * m6);
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
		// Pitch: atan(-m[9] / m[10]) = atan(SxCy/CxCy)
		// Yaw  : asin(m[8]) = asin(Sy)
		// Roll : atan(-m[4] / m[0]) = atan(SzCy/CzCy)
		///////////////////////////////////////////////////////////////////////////////
		Vec3<float> Matrix4::getAngle() const
		{
			float pitch, yaw, roll;         // 3 angles

			// find yaw (around y-axis) first
			// NOTE: asin() returns -90~+90, so correct the angle range -180~+180
			// using z value of forward vector
			yaw = RAD2DEG * asinf(m[8]);
			if (m[10] < 0)
			{
				if (yaw >= 0) yaw = 180.0f - yaw;
				else         yaw = -180.0f - yaw;
			}

			// find roll (around z-axis) and pitch (around x-axis)
			// if forward vector is (1,0,0) or (-1,0,0), then m[0]=m[4]=m[9]=m[10]=0
			if (m[0] > -EPSILON && m[0] < EPSILON)
			{
				roll = 0;  //@@ assume roll=0
				pitch = RAD2DEG * atan2f(m[1], m[5]);
			}
			else
			{
				roll = RAD2DEG * atan2f(-m[4], m[0]);
				pitch = RAD2DEG * atan2f(-m[9], m[10]);
			}

			return Vec3<float>(pitch, yaw, roll);
		}

		// ��������
		Mat4<value_type>& identity()		// ��λ����
		{
			m_aM[0] = m_aM[5] = m_aM[10] = m_aM[15](value_type)1;
			m_aM[1] = m_aM[2] = m_aM[3] = m_aM[4] = m_aM[6] = m_aM[7] = m_aM[8] = 
				m_aM[9] = m_aM[11] = m_aM[12] = m_aM[13] = m_aM[14] = (value_type)0;
			return *this;
		}
		const Mat4<value_type> transpose()   // ����ת��
		{
			return Mat4<value_type>(m_aM[0], m_aM[4], m_aM[8], m_aM[12], 
				m_aM[1], m_aM[5], m_aM[9], m_aM[13], 
				m_aM[2], m_aM[6], m_aM[10], m_aM[14],
				m_aM[3], m_aM[7], m_aM[11], m_aM[15]);
		}
		///////////////////////////////////////////////////////////////////////////////
		// inverse 3x3 matrix
		// If cannot find inverse (det=0), set identity matrix
		// M^-1 = adj(M) / det(M)
		//        | m4m8-m5m7  m7m2-m8m1  m1m5-m2m4 |
		//      = | m5m6-m3m8  m0m8-m2m6  m2m3-m0m5 | / det(M)
		//        | m3m7-m4m6  m6m1-m7m0  m0m4-m1m3 |
		///////////////////////////////////////////////////////////////////////////////
		const Mat4<value_type> invert()		// �����(�����ڷ��ص�λ����)�����ڸ�����
		{
			Mat4<value_type> inv;
			// If the 4th row is [0,0,0,1] then it is affine matrix and
			// it has no projective transformation.
			if (m_aM[3] == 0 && m_aM[7] == 0 && m_aM[11] == 0 && m_aM[15] == 1)
				inv = Mat4::invertAffine();
			else
			{
				inv = Mat4::invertGeneral();
				/*@@ invertProjective() is not optimized (slower than generic one)
				if(fabs(m[0]*m[5] - m[1]*m[4]) > EPSILON)
				this->invertProjective();   // inverse using matrix partition
				else
				this->invertGeneral();      // generalized inverse
				*/
			}

			return inv;
		}
		// �������任�����
		///////////////////////////////////////////////////////////////////////////////
		// compute the inverse of a 4x4 affine transformation matrix
		//
		// Affine transformations are generalizations of Euclidean transformations.
		// Affine transformation includes translation, rotation, reflection, scaling,
		// and shearing. Length and angle are NOT preserved.
		// M = [ R | T ]
		//     [ --+-- ]    (R denotes 3x3 rotation/scale/shear matrix)
		//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
		//
		// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^-1*y - R^-1*T
		//
		//  [ R | T ]-1   [ R^-1 | -R^-1 * T ]
		//  [ --+-- ]   = [ -----+---------- ]
		//  [ 0 | 1 ]     [  0   +     1     ]
		///////////////////////////////////////////////////////////////////////////////
		const Mat4<value_type> invertAffine()
		{
			// R^-1
			Mat3<value_type> rotation(m_aM[0], m_aM[1], m_aM[2], m_aM[4], m_aM[5], m_aM[6], m_aM[8], m_aM[9], m_aM[10]);
			Mat3<value_type> r = rotation.invert();
			Mat4<value_type> m;
			m[0] = r[0];  m[1] = r[1];  m[2] = r[2];
			m[4] = r[3];  m[5] = r[4];  m[6] = r[5];
			m[8] = r[6];  m[9] = r[7];  m[10] = r[8];

			// -R^-1 * T
			value_type x = m_aM[12];
			value_type y = m_aM[13];
			value_type z = m_aM[14];
			m[12] = -(r[0] * x + r[3] * y + r[6] * z);
			m[13] = -(r[1] * x + r[4] * y + r[7] * z);
			m[14] = -(r[2] * x + r[5] * y + r[8] * z);

			// last row should be unchanged (0,0,0,1)
			//m[3] = m[7] = m[11] = 0.0f;
			//m[15] = 1.0f;

			return m;
		}
		// ����4x4����������
		///////////////////////////////////////////////////////////////////////////////
		// compute the inverse of a general 4x4 matrix using Cramer's Rule
		// If cannot find inverse, return indentity matrix
		// M^-1 = adj(M) / det(M)
		///////////////////////////////////////////////////////////////////////////////
		const Mat4<value_type> invertGeneral()
		{
			// get cofactors of minor matrices
			value_type cofactor0 = getCofactor(m_aM[5], m_aM[6], m_aM[7], m_aM[9], m_aM[10], m_aM[11], m_aM[13], m_aM[14], m_aM[15]);
			value_type cofactor1 = getCofactor(m_aM[4], m_aM[6], m_aM[7], m_aM[8], m_aM[10], m_aM[11], m_aM[12], m_aM[14], m_aM[15]);
			value_type cofactor2 = getCofactor(m_aM[4], m_aM[5], m_aM[7], m_aM[8], m_aM[9], m_aM[11], m_aM[12], m_aM[13], m_aM[15]);
			value_type cofactor3 = getCofactor(m_aM[4], m_aM[5], m_aM[6], m_aM[8], m_aM[9], m_aM[10], m_aM[12], m_aM[13], m_aM[14]);

			// get determinant
			value_type determinant = m_aM[0] * cofactor0 - m_aM[1] * cofactor1 + m_aM[2] * cofactor2 - m_aM[3] * cofactor3;
			Mat4<value_type> m;
			if (fabs(determinant) <= EPSILON)
			{
				return m;
			}

			// get rest of cofactors for adj(M)
			value_type cofactor4 = getCofactor(m_aM[1], m_aM[2], m_aM[3], m_aM[9], m_aM[10], m_aM[11], m_aM[13], m_aM[14], m_aM[15]);
			value_type cofactor5 = getCofactor(m_aM[0], m_aM[2], m_aM[3], m_aM[8], m_aM[10], m_aM[11], m_aM[12], m_aM[14], m_aM[15]);
			value_type cofactor6 = getCofactor(m_aM[0], m_aM[1], m_aM[3], m_aM[8], m_aM[9], m_aM[11], m_aM[12], m_aM[13], m_aM[15]);
			value_type cofactor7 = getCofactor(m_aM[0], m_aM[1], m_aM[2], m_aM[8], m_aM[9], m_aM[10], m_aM[12], m_aM[13], m_aM[14]);

			value_type cofactor8 = getCofactor(m_aM[1], m_aM[2], m_aM[3], m_aM[5], m_aM[6], m_aM[7], m_aM[13], m_aM[14], m_aM[15]);
			value_type cofactor9 = getCofactor(m_aM[0], m_aM[2], m_aM[3], m_aM[4], m_aM[6], m_aM[7], m_aM[12], m_aM[14], m_aM[15]);
			value_type cofactor10 = getCofactor(m_aM[0], m_aM[1], m_aM[3], m_aM[4], m_aM[5], m_aM[7], m_aM[12], m_aM[13], m_aM[15]);
			value_type cofactor11 = getCofactor(m_aM[0], m_aM[1], m_aM[2], m_aM[4], m_aM[5], m_aM[6], m_aM[12], m_aM[13], m_aM[14]);

			value_type cofactor12 = getCofactor(m_aM[1], m_aM[2], m_aM[3], m_aM[5], m_aM[6], m_aM[7], m_aM[9], m_aM[10], m_aM[11]);
			value_type cofactor13 = getCofactor(m_aM[0], m_aM[2], m_aM[3], m_aM[4], m_aM[6], m_aM[7], m_aM[8], m_aM[10], m_aM[11]);
			value_type cofactor14 = getCofactor(m_aM[0], m_aM[1], m_aM[3], m_aM[4], m_aM[5], m_aM[7], m_aM[8], m_aM[9], m_aM[11]);
			value_type cofactor15 = getCofactor(m_aM[0], m_aM[1], m_aM[2], m_aM[4], m_aM[5], m_aM[6], m_aM[8], m_aM[9], m_aM[10]);

			// build inverse matrix = adj(M) / det(M)
			// adjugate of M is the transpose of the cofactor matrix of M
			value_type invDeterminant = (value_type)1.0 / determinant;
			m[0] = invDeterminant * cofactor0;
			m[1] = -invDeterminant * cofactor4;
			m[2] = invDeterminant * cofactor8;
			m[3] = -invDeterminant * cofactor12;

			m[4] = -invDeterminant * cofactor1;
			m[5] = invDeterminant * cofactor5;
			m[6] = -invDeterminant * cofactor9;
			m[7] = invDeterminant * cofactor13;

			m[8] = invDeterminant * cofactor2;
			m[9] = -invDeterminant * cofactor6;
			m[10] = invDeterminant * cofactor10;
			m[11] = -invDeterminant * cofactor14;

			m[12] = -invDeterminant * cofactor3;
			m[13] = invDeterminant * cofactor7;
			m[14] = -invDeterminant * cofactor11;
			m[15] = invDeterminant * cofactor15;

			return m;
		}
		// ����ŷ���任����������
		///////////////////////////////////////////////////////////////////////////////
		// compute the inverse of 4x4 Euclidean transformation matrix
		//
		// Euclidean transformation is translation, rotation, and reflection.
		// With Euclidean transform, only the position and orientation of the object
		// will be changed. Euclidean transform does not change the shape of an object
		// (no scaling). Length and angle are reserved.
		//
		// Use inverseAffine() if the matrix has scale and shear transformation.
		//
		// M = [ R | T ]
		//     [ --+-- ]    (R denotes 3x3 rotation/reflection matrix)
		//     [ 0 | 1 ]    (T denotes 1x3 translation matrix)
		//
		// y = M*x  ->  y = R*x + T  ->  x = R^-1*(y - T)  ->  x = R^T*y - R^T*T
		// (R is orthogonal,  R^-1 = R^T)
		//
		//  [ R | T ]-1    [ R^T | -R^T * T ]    (R denotes 3x3 rotation matrix)
		//  [ --+-- ]   =  [ ----+--------- ]    (T denotes 1x3 translation)
		//  [ 0 | 1 ]      [  0  |     1    ]    (R^T denotes R-transpose)
		///////////////////////////////////////////////////////////////////////////////
		const Mat4<value_type> invertEuclidean()
		{
			Mat4<value_type> inv(*this);
			// transpose 3x3 rotation matrix part
			// | R^T | 0 |
			// | ----+-- |
			// |  0  | 1 |
			std::swap(inv[1], inv[4]);
			std::swap(inv[2], inv[8]);
			std::swap(inv[6], inv[9]);

			// compute translation part -R^T * T
			// | 0 | -R^T x |
			// | --+------- |
			// | 0 |   0    |
			value_type x = inv[12];
			value_type y = inv[13];
			value_type z = inv[14];
			inv[12] = -(inv[0] * x + inv[4] * y + inv[8] * z);
			inv[13] = -(inv[1] * x + inv[5] * y + inv[9] * z);
			inv[14] = -(inv[2] * x + inv[6] * y + inv[10] * z);

			// last row should be unchanged (0,0,0,1)

			return inv;
		}
		///////////////////////////////////////////////////////////////////////////////
		// inverse matrix using matrix partitioning (blockwise inverse)
		// It devides a 4x4 matrix into 4 of 2x2 matrices. It works in case of where
		// det(A) != 0. If not, use the generic inverse method
		// inverse formula.
		// M = [ A | B ]    A, B, C, D are 2x2 matrix blocks
		//     [ --+-- ]    det(M) = |A| * |D - ((C * A^-1) * B)|
		//     [ C | D ]
		//
		// M^-1 = [ A' | B' ]   A' = A^-1 - (A^-1 * B) * C'
		//        [ ---+--- ]   B' = (A^-1 * B) * -D'
		//        [ C' | D' ]   C' = -D' * (C * A^-1)
		//                      D' = (D - ((C * A^-1) * B))^-1
		//
		// NOTE: I wrap with () if it it used more than once.
		//       The matrix is invertable even if det(A)=0, so must check det(A) before
		//       calling this function, and use invertGeneric() instead.
		///////////////////////////////////////////////////////////////////////////////
		const Mat4<value_type> invertProjective()
		{
			// partition
			Mat2<value_type> a(m_aM[0], m_aM[1], m_aM[4], m_aM[5]);
			Mat2<value_type> b(m_aM[8], m_aM[9], m_aM[12], m_aM[13]);
			Mat2<value_type> c(m_aM[2], m_aM[3], m_aM[6], m_aM[7]);
			Mat2<value_type> d(m_aM[10], m_aM[11], m_aM[14], m_aM[15]);

			// pre-compute repeated parts
			Mat2<value_type> inva = a.invert();             // A^-1
			Mat2<value_type> ab = inva * b;     // A^-1 * B
			Mat2<value_type> ca = c * inva;     // C * A^-1
			Mat2<value_type> cab = ca * b;   // C * A^-1 * B
			Mat2<value_type> dcab = d - cab; // D - C * A^-1 * B

			// check determinant if |D - C * A^-1 * B| = 0
			//NOTE: this function assumes det(A) is already checked. if |A|=0 then,
			//      cannot use this function.
			value_type determinant = dcab[0] * dcab[3] - dcab[1] * dcab[2];
			Mat4<value_type> m;
			if (fabs(determinant) <= EPSILON)
			{
				return m;
			}

			// compute D' and -D'
			Mat2<value_type> d1 = dcab;      //  (D - C * A^-1 * B)
			Mat2<value_type> invd1 = d1.invert();            //  (D - C * A^-1 * B)^-1
			Mat2<value_type> d2 = -invd1;       // -(D - C * A^-1 * B)^-1

			// compute C'
			Mat2<value_type> c1 = d2 * ca;   // -D' * (C * A^-1)

			// compute B'
			Mat2<value_type> b1 = ab * d2;   // (A^-1 * B) * -D'

			// compute A'
			Mat2<value_type> a1 = inva - (ab * c1); // A^-1 - (A^-1 * B) * C'

			// assemble inverse matrix
			m[0] = a1[0];  m[4] = a1[2]; /*|*/ m[8] = b1[0];  m[12] = b1[2];
			m[1] = a1[1];  m[5] = a1[3]; /*|*/ m[9] = b1[1];  m[13] = b1[3];
			/*-----------------------------+-----------------------------*/
			m[2] = c1[0];  m[6] = c1[2]; /*|*/ m[10] = invd1[0];  m[14] = invd1[2];
			m[3] = c1[1];  m[7] = c1[3]; /*|*/ m[11] = invd1[1];  m[15] = invd1[3];

			return m;
		}

		//////////////////// �任///////////////////////////////
		// ƽ��
		const Mat4<value_type> translate(const Vec3<value_type>& v)	// ƽ��
		{
			return Mat4::translate(v.x(), v.y(), v.z());
		}
		const Mat4<value_type> translate(value_type x, value_type y, value_type z)	// ƽ��
		{
			return Mat4<value_type>(m_aM[0] + m_aM[3] * x, m_aM[1] + m_aM[3] * y, m_aM[2] + m_aM[3] * z, m_aM[3],
				m_aM[4] + m_aM[7] * x, m_aM[5] + m_aM[7] * y, m_aM[6] + m_aM[7] * z, m_aM[7],
				m_aM[8] + m_aM[11] * x, m_aM[9] + m_aM[11] * y, m_aM[10] + m_aM[11] * z, m_aM[11], 
				m_aM[12] + m_aM[15] * x, m_aM[13] + m_aM[15] * y, m_aM[14] + m_aM[15] * z, m_aM[15]);
		}
		
		// ��һ������
		const Mat4<value_type> scale(value_type s)
		{
			return Mat4::scale(s, s, s);
		}

		const Mat4<value_type> scale(value_type x, value_type y, value_type z)
		{
			return Mat4<value_type>(m_aM[0] * x, m_aM[1] * y, m_aM[2]*z,
				m_aM[4] * x, m_aM[5] * y, m_aM[6]*z, 
				m_aM[8] * x, m_aM[9] * y, m_aM[10]*z,
				m_aM[12] * x, m_aM[13] * y, m_aM[14]*z);
		}

		// ��ת���Ƹ���������ת���ٽǶȣ�
		const Mat4<value_type> rotate(value_type angle, const Vec3<value_type>& axis)
		{
			return Mat4::rotate(angle, axis.x, axis.y, axis.z);
		}

		const Mat4<value_type> rotate(value_type angle, value_type x, value_type y, value_type z)
		{
			value_type c = cos(cell::DegreesToRadians(angle));    // cosine
			value_type s = sin(cell::DegreesToRadians(angle));    // sine
			value_type c1 = (value_type)1.0 - c;                // 1 - c
			value_type m0 = m_aM[0], m4 = m_aM[4], m8 = m_aM[8], m12 = m_aM[12],
				m1 = m_aM[1], m5 = m_aM[5], m9 = m_aM[9], m13 = m_aM[13],
				m2 = m_aM[2], m6 = m_aM[6], m10 = m_aM[10], m14 = m_aM[14];

			// build rotation matrix
			value_type r0 = x * x * c1 + c;
			value_type r1 = x * y * c1 + z * s;
			value_type r2 = x * z * c1 - y * s;
			value_type r4 = x * y * c1 - z * s;
			value_type r5 = y * y * c1 + c;
			value_type r6 = y * z * c1 + x * s;
			value_type r8 = x * z * c1 + y * s;
			value_type r9 = y * z * c1 - x * s;
			value_type r10 = z * z * c1 + c;

			// multiply rotation matrix
			Mat4<value_type> rot;
			rot[0] = r0 * m0 + r4 * m1 + r8 * m2;
			rot[1] = r1 * m0 + r5 * m1 + r9 * m2;
			rot[2] = r2 * m0 + r6 * m1 + r10* m2;
			rot[4] = r0 * m4 + r4 * m5 + r8 * m6;
			rot[5] = r1 * m4 + r5 * m5 + r9 * m6;
			rot[6] = r2 * m4 + r6 * m5 + r10* m6;
			rot[8] = r0 * m8 + r4 * m9 + r8 * m10;
			rot[9] = r1 * m8 + r5 * m9 + r9 * m10;
			rot[10] = r2 * m8 + r6 * m9 + r10* m10;
			rot[12] = r0 * m12 + r4 * m13 + r8 * m14;
			rot[13] = r1 * m12 + r5 * m13 + r9 * m14;
			rot[14] = r2 * m12 + r6 * m13 + r10* m14;

			return rot;
		}
		const Mat4<value_type> rotateX(value_type angle)
		{
			value_type c = cos(cell::DegreesToRadians(angle));    // cosine
			value_type s = sin(cell::DegreesToRadians(angle));    // sine
			value_type m1 = m_aM[1], m2 = m_aM[2],
				m5 = m_aM[5], m6 = m_aM[6],
				m9 = m_aM[9], m10 = m_aM[10],
				m13 = m_aM[13], m14 = m_aM[14];

			Mat4<value_type> rot;
			rot[1] = m1 * c + m2 *-s;
			rot[2] = m1 * s + m2 * c;
			rot[5] = m5 * c + m6 *-s;
			rot[6] = m5 * s + m6 * c;
			rot[9] = m9 * c + m10*-s;
			rot[10] = m9 * s + m10* c;
			rot[13] = m13* c + m14*-s;
			rot[14] = m13* s + m14* c;

			return rot;
		}

		const Mat4<value_type> rotateY(value_type angle)
		{
			value_type c = cos(cell::DegreesToRadians(angle));    // cosine
			value_type s = sin(cell::DegreesToRadians(angle));    // sine
			value_type m0 = m_aM[0], m2 = m_aM[2],
				m4 = m_aM[4], m6 = m_aM[6],
				m8 = m_aM[8], m10 = m_aM[10],
				m12 = m_aM[12], m14 = m_aM[14];

			Mat4<value_type> rot;
			rot[0] = m0 * c + m2 * s;
			rot[2] = m0 *-s + m2 * c;
			rot[4] = m4 * c + m6 * s;
			rot[6] = m4 *-s + m6 * c;
			rot[8] = m8 * c + m10* s;
			rot[10] = m8 *-s + m10* c;
			rot[12] = m12* c + m14* s;
			rot[14] = m12*-s + m14* c;

			return rot;
		}

		const Mat4<value_type> rotateZ(value_type angle)
		{
			value_type c = cos(cell::DegreesToRadians(angle));    // cosine
			value_type s = sin(cell::DegreesToRadians(angle));    // sine
			value_type m0 = m_aM[0], m1 = m_aM[1],
				m4 = m_aM[4], m5 = m_aM[5],
				m8 = m_aM[8], m9 = m_aM[9],
				m12 = m_aM[12], m13 = m_aM[13];

			Mat4<value_type> rot;
			rot[0] = m0 * c + m1 *-s;
			rot[1] = m0 * s + m1 * c;
			rot[4] = m4 * c + m5 *-s;
			rot[5] = m4 * s + m5 * c;
			rot[8] = m8 * c + m9 *-s;
			rot[9] = m8 * s + m9 * c;
			rot[12] = m12* c + m13*-s;
			rot[13] = m12* s + m13* c;

			return rot;
		}
		// lookat����
		const Mat4<value_type> lookAt(const Vec3<value_type>& target)
		{
			// compute forward vector and normalize
			Vec3<value_type> position = Vec3<value_type>(m_aM[12], m_aM[13], m_aM[14]);
			Vec3<value_type> forward = target - position;
			forward.normalize();
			Vec3<value_type> up;             // up vector of object
			Vec3<value_type> left;           // left vector of object

			// compute temporal up vector
			// if forward vector is near Y-axis, use up vector (0,0,-1) or (0,0,1)
			if (fabs(forward[0]) < EPSILON && fabs(forward[2]) < EPSILON)
			{
				// forward vector is pointing +Y axis
				if (forward[1] > 0)
					up.set(0, 0, -1);
				// forward vector is pointing -Y axis
				else
					up.set(0, 0, 1);
			}
			else
			{
				// assume up vector is +Y axis
				up.set(0, 1, 0);
			}

			// compute left vector
			left = up.cross(forward);
			left.normalize();

			// re-compute up vector
			up = forward.cross(left);
			//up.normalize();

			// NOTE: overwrite rotation and scale info of the current matrix
			Mat4<value_type> lkm;
			lkm->setColumn(0, left);
			lkm->setColumn(1, up);
			lkm->setColumn(2, forward);

			return lkm;
		}
		const Mat4<value_type> lookAt(const Vec3<value_type>& target, const Vec3<value_type>& upVec)
		{
			// compute forward vector and normalize
			Vec3<value_type> position = Vec3<value_type>(m_aM[12], m_aM[13], m_aM[14]);
			Vec3<value_type> forward = target - position;
			forward.normalize();

			// compute left vector
			Vec3<value_type> left = upVec.cross(forward);
			left.normalize();

			// compute orthonormal up vector
			Vec3<value_type> up = forward.cross(left);
			up.normalize();

			// NOTE: overwrite rotation and scale info of the current matrix
			Mat4<value_type> lkm;
			lkm->setColumn(0, left);
			lkm->setColumn(1, up);
			lkm->setColumn(2, forward);

			return lkm;
		}

		const Mat4<value_type> lookAt(value_type tx, value_type ty, value_type tz)
		{
			return lookAt(Vec3<value_type>(tx, ty, tz));
		}

		const Mat4<value_type> lookAt(value_type tx, value_type ty, value_type tz, value_type ux, value_type uy, value_type uz)
		{
			return lookAt(Vec3<value_type>(tx, ty, tz), Vec3<value_type>(ux, uy, uz));
		}
		//////////////////// END �任 //////////////////////////////

		///////////////////////////////////////////////////////////////////////////////
		// return 3x3 matrix containing rotation only
		///////////////////////////////////////////////////////////////////////////////
		Mat3<value_type> getRotationMatrix() const
		{
			return Mat3<value_type>(
				m_aM[0], m_aM[1], m_aM[2],
				m_aM[4], m_aM[5], m_aM[6],
				m_aM[8], m_aM[9], m_aM[10]);
		}
		// operators
		const Mat4<value_type>& operator = (const Mat4<value_type>& rhs)
		{
			m_aM[0] = rhs[0]; m_aM[1] = rhs[1]; m_aM[2] = rhs[2]; m_aM[3] = rhs[3];
			m_aM[4] = rhs[4]; m_aM[5] = rhs[5]; m_aM[6] = rhs[6]; m_aM[7] = rhs[7];
			m_aM[8] = rhs[8]; m_aM[9] = rhs[9]; m_aM[10] = rhs[10]; m_aM[11] = rhs[11];
			m_aM[12] = rhs[12]; m_aM[13] = rhs[13]; m_aM[14] = rhs[14]; m_aM[15] = rhs[15];
			return *this;
		}
		const Mat4<value_type> operator+(const Mat4<value_type>& rhs) const    // add rhs
		{
			return Mat4<value_type>(m_aM[0] + rhs[0], m_aM[1] + rhs[1], m_aM[2] + rhs[2], m_aM[3] + rhs[3], 
				m_aM[4] + rhs[4], m_aM[5] + rhs[5], m_aM[6] + rhs[6], m_aM[7] + rhs[7], 
				m_aM[8] + rhs[8], m_aM[9] + rhs[9], m_aM[10] + rhs[10], m_aM[11] + rhs[11], 
				m_aM[12] + rhs[12], m_aM[13] + rhs[13], m_aM[14] + rhs[14], m_aM[15] + rhs[15]);
		}
		const Mat4<value_type> operator-(const Mat4<value_type>& rhs) const    // subtract rhs
		{
			return Mat4<value_type>(m_aM[0] - rhs[0], m_aM[1] - rhs[1], m_aM[2] - rhs[2], m_aM[3] - rhs[3],
				m_aM[4] - rhs[4], m_aM[5] - rhs[5], m_aM[6] - rhs[6], m_aM[7] - rhs[7],
				m_aM[8] - rhs[8], m_aM[9] - rhs[9], m_aM[10] - rhs[10], m_aM[11] - rhs[11],
				m_aM[12] - rhs[12], m_aM[13] - rhs[13], m_aM[14] - rhs[14], m_aM[15] - rhs[15]);
		}
		Mat4<value_type>& operator+=(const Mat4<value_type>& rhs)         // add rhs and update this object
		{
			m_aM[0] += rhs[0]; m_aM[1] += rhs[1]; m_aM[2] += rhs[2]; m_aM[3] += rhs[3]; 
			m_aM[4] += rhs[4]; m_aM[5] += rhs[5]; m_aM[6] += rhs[6]; m_aM[7] += rhs[7]; 
			m_aM[8] += rhs[8]; m_aM[9] += rhs[9]; m_aM[10] += rhs[10]; m_aM[11] += rhs[11];
			m_aM[12] += rhs[12]; m_aM[13] += rhs[13]; m_aM[14] += rhs[14]; m_aM[15] += rhs[15];
			return *this;
		}
		Mat4<value_type>& operator-=(const Mat4<value_type>& rhs)         // subtract rhs and update this object
		{
			m_aM[0] -= rhs[0]; m_aM[1] -= rhs[1]; m_aM[2] -= rhs[2]; m_aM[3] -= rhs[3];
			m_aM[4] -= rhs[4]; m_aM[5] -= rhs[5]; m_aM[6] -= rhs[6]; m_aM[7] -= rhs[7];
			m_aM[8] -= rhs[8]; m_aM[9] -= rhs[9]; m_aM[10] -= rhs[10]; m_aM[11] -= rhs[11];
			m_aM[12] -= rhs[12]; m_aM[13] -= rhs[13]; m_aM[14] -= rhs[14]; m_aM[15] -= rhs[15];
			return *this;
		}
		const Vec4<value_type> operator*(const Vec4<value_type>& rhs) const    // multiplication: v' = M * v
		{
			return Vec4<value_type>(m_aM[0] * rhs[0] + m_aM[4] * rhs[1] + m_aM[8] * rhs[2] + m_aM[12] * rhs[3],
				m_aM[1] * rhs[0] + m_aM[5] * rhs[1] + m_am[9] * rhs[2] + m_aM[13] * rhs[3],
				m_aM[2] * rhs[0] + m_aM[6] * rhs[1] + m_am[10] * rhs[2] + m_aM[14] * rhs[3],
				m_aM[3] * rhs[0] + m_aM[7] * rhs[1] + m_am[11] * rhs[2] + m_aM[15] * rhs[3]);
		}
		const Vec3<value_type> Matrix4::operator*(const Vec3<value_type>& rhs) const
		{
			return Vec3<value_type>(m_aM[0] * rhs[0] + m_aM[4] * rhs[1] + m_aM[8] * rhs[2] + m_aM[12],
				m_aM[1] * rhs[0] + m_aM[5] * rhs[1] + m_aM[9] * rhs[2] + m_aM[13],
				m_aM[2] * rhs[0] + m_aM[6] * rhs[1] + m_aM[10] * rhs[2] + m_aM[14]);
		}
		const Mat4<value_type> operator*(value_type rhs) const
		{
			return Mat4<value_type>(m_aM[0] * rhs, m_aM[1] * rhs, m_aM[2] * rhs, m_aM[3] * rhs,
				m_aM[4] * rhs, m_aM[5] * rhs, m_aM[6] * rhs, m_aM[7] * rhs,
				m_aM[8] * rhs, m_aM[9] * rhs, m_aM[10] * rhs, m_aM[11] * rhs,
				m_aM[12] * rhs, m_aM[13] * rhs, m_aM[14] * rhs, m_aM[15] * rhs);
		}
		const Mat4<value_type> operator*(const Mat4<value_type>& rhs) const    // multiplication: M3 = M1 * M2
		{
			return Mat4<value_type>(m_aM[0] * rhs[0] + m_aM[4] * rhs[1] + m_aM[8] * rhs[2] + m_aM[12] * rhs[3],
				m_aM[1] * rhs[0] + m_aM[5] * rhs[1] + m_aM[9] * rhs[2] + m_aM[13] * rhs[3],
				m_aM[2] * rhs[0] + m_aM[6] * rhs[1] + m_aM[10] * rhs[2] + m_aM[11] * rhs[3],
				m_aM[3] * rhs[0] + m_aM[7] * rhs[1] + m_aM[11] * rhs[2] + m_aM[12] * rhs[3],
				m_aM[0] * rhs[4] + m_aM[4] * rhs[5] + m_aM[8] * rhs[6] + m_aM[12] * rhs[7],
				m_aM[1] * rhs[4] + m_aM[5] * rhs[5] + m_aM[9] * rhs[6] + m_aM[13] * rhs[7],
				m_aM[2] * rhs[4] + m_aM[6] * rhs[5] + m_aM[10] * rhs[6] + m_aM[14] * rhs[7],
				m_aM[3] * rhs[4] + m_aM[7] * rhs[5] + m_aM[11] * rhs[6] + m_aM[15] * rhs[7],
				m_aM[0] * rhs[8] + m_aM[4] * rhs[9] + m_aM[8] * rhs[10] + m_aM[12] * rhs[11],
				m_aM[1] * rhs[8] + m_aM[5] * rhs[9] + m_aM[9] * rhs[10] + m_aM[13] * rhs[11],
				m_aM[2] * rhs[8] + m_aM[6] * rhs[9] + m_aM[10] * rhs[10] + m_aM[14] * rhs[11],
				m_aM[3] * rhs[8] + m_aM[7] * rhs[9] + m_aM[11] * rhs[10] + m_aM[15] * rhs[11],
				m_aM[0] * rhs[12] + m_aM[4] * rhs[13] + m_aM[8] * rhs[14] + m_aM[12] * rhs[15],
				m_aM[1] * rhs[12] + m_aM[5] * rhs[13] + m_aM[9] * rhs[14] + m_aM[13] * rhs[15],
				m_aM[2] * rhs[12] + m_aM[6] * rhs[13] + m_aM[10] * rhs[14] + m_aM[14] * rhs[15],
				m_aM[3] * rhs[12] + m_aM[7] * rhs[13] + m_aM[11] * rhs[14] + m_aM[15] * rhs[15]);
		}
		Mat4<value_type>& operator*=(const Mat4<value_type>& rhs)         // multiplication: M1' = M1 * M2
		{
			*this = *this * rhs; return *this;
		}
		bool operator==(const Mat4<value_type>& rhs) const   // exact compare, no epsilon
		{
			return (m_aM[0] == rhs[0] && m_aM[1] == rhs[1] && m_aM[2] == rhs[2] && m_aM[3] == rhs[3] 
				&& m_aM[4] == rhs[4] && m_aM[5] == rhs[5] && m_aM[6] == rhs[6] && m_aM[7] == rhs[7] 
				&& m_aM[8] == rhs[8] && m_aM[9] == rhs[9] && m_aM[10] == rhs[10] && m_aM[11] == rhs[11]
				&& m_aM[12] == rhs[12] && m_aM[13] == rhs[13] && m_aM[14] == rhs[14] && m_aM[15] == rhs[15]);
		}
		bool operator!=(const Mat4<value_type>& rhs) const   // exact compare, no epsilon
		{
			return (m_aM[0] != rhs[0] || m_aM[1] != rhs[1] || m_aM[2] != rhs[2] || m_aM[3] != rhs[3]
				|| m_aM[4] != rhs[4] || m_aM[5] != rhs[5] || m_aM[6] != rhs[6] || m_aM[7] != rhs[7]
				|| m_aM[8] != rhs[8] || m_aM[9] != rhs[9] || m_aM[10] != rhs[10] || m_aM[11] != rhs[11]
				|| m_aM[12] != rhs[12] || m_aM[13] != rhs[13] || m_aM[14] != rhs[14] || m_aM[15] != rhs[15]);
		}
		value_type operator[](unsigned int index) const           // subscript operator v[0], v[1]
		{
			return m_aM[index];
		}
		value_type& operator[](unsigned int index)                 // subscript operator v[0], v[1]
		{
			return m_aM[index];
		}
		const Mat4<value_type> operator-()                     // unary operator (-)
		{
			return Mat4<value_type>(-m_aM[0], -m_aM[1], -m_aM[2], -m_aM[3],
				-m_aM[4], -m_aM[5], -m_aM[6], -m_aM[7],
				-m_aM[8], -m_aM[9], -m_aM[10], -m_aM[11],
				-m_aM[12], -m_aM[13], -m_aM[14], -m_aM[15]);
		}

		// friends functions
		template <typename T>
		friend Mat4<T> operator*(T scalar, const Mat4<T>& rhs)       // pre-multiplication
		{
			return rhs * scalar;
		}
		template <typename T>
		friend Vec4<T> operator*(const Vec4<T>& vec, const Mat4<T>& rhs) // pre-multiplication
		{
			return rhs * vec;
		}
		template <typename T>
		friend std::ostream& operator<<(std::ostream& os, const Mat4<T>& m)
		{
			os << std::fixed << std::setprecision(5);
			os << "[" << std::setw(10) << m[0] << " " << std::setw(10) << m[4] << " " << std::setw(10) << m[8] << " " << std::setw(10) << m[12] << "]\n"
				<< "[" << std::setw(10) << m[1] << " " << std::setw(10) << m[5] << " " << std::setw(10) << m[9] << " " << std::setw(10) << m[13] << "]\n"
				<< "[" << std::setw(10) << m[2] << " " << std::setw(10) << m[6] << " " << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << "]\n"
				<< "[" << std::setw(10) << m[3] << " " << std::setw(10) << m[7] << " " << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << "]\n";
			os << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
			return os;
		}

		// static functions
	private:
		value_type m_aM[num_components];
	};
}
