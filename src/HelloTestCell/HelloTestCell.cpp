#include <iostream>

#include "CellMat3.h"
#include "CellMat4.h"
#include "Timer.h"

using namespace std;
using namespace cell;

int main(int argc, char *argv[])
{
	// Mat3<float> usage example ==================================================
	Mat3<float> m3_1(1, 0, -2, -1, -2, -3, 1, 1, 0);  // column-major
	Mat3<float> m3_2 = m3_1;                    // copy from m3_1
	m3_2.invert();                          // invert matrix
	std::cout << "3x3 original matrix:\n" << m3_1 << std::endl;
	std::cout << "3x3 inverse matrix:\n" << m3_2 << std::endl;

	// Mat4<float> usage examples =================================================
	// Note that Mat4<float> is column-major order
	std::cout << "\n ===== 4x4 Matrix =====\n" << std::endl;

	// create (construct) matrix
	float a[16] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
	Mat4<float> m1;                                             // with default ctor
	Mat4<float> m2(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);            // with 16 elements
	Mat4<float> m3(a);                                          // with array
	Mat4<float> m4(m3);                                         // with copy ctor, same as Mat4<float> m4 = m3;
	//std::cout << m4 << std::endl;

	// accessors (getter/setter)
	m1.set(a);                                              // init with array
	m2.set(3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3);             // init with 16 elements
	m3 = m2;                                                // init with assignemt operator (=)
	const float* a1 = m1.get();                             // return pointer to matrix elements
	const float* a2 = m2.transpose().get();                    // return transposed matrix elements

	// subscript operator [] to access each element
	m1[0] = 3;
	std::cout << "FIRST ELEMENT: " << m1[0] << std::endl;
	std::cout << " LAST ELEMENT: " << m1[15] << "\n\n";

	// comparison (exact)
	if (m1 == m2)
		std::cout << "COMPARISON: EQUAL\n\n";
	else
		std::cout << "COMPARISON: NOT EQUAL\n\n";

	// addition
	m3 = m1 + m2;                                           // M3 = M1 + M2
	std::cout << "ADD:\n" << m3 << std::endl;

	// subtraction
	m3 = m1 - m2;                                           // M3 = M1 - M2
	std::cout << "SUBTRACT:\n" << m3 << std::endl;

	// multiplication
	m3 = m1 * m2;                                           // M3 = M1 * M2
	m3 *= m1;                                               // M3 = M3 * M1 (= glMultMatrixf(M1))
	std::cout << "MULTIPLY:\n" << m3 << std::endl;

	// scalar product (pre-mult only, not available M * s)
	m3 = 5.0f * m1;                                            // s * M
	std::cout << "SCALAR PRODUCT:\n" << m3 << std::endl;

	// vector multiplication
	Vec3<float> v1 = Vec3<float>(2, 2, 2);
	Vec3<float> v2;
	v2 = m1 * v1;                                           // vector product: M * v
	std::cout << "VECTOR MULTIPLY 1: " << v2 << std::endl;

	// transpose
	m3.set(1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4);
	m3.transpose();                                         // transpose
	std::cout << "TRANSPOSE:\n" << m3 << std::endl;

	// determinant
	m3.set(1, 3, 2, 1, 0, 0, 1, 0, 2, 0, 4, 5, -1, 5, -3, 0);
	std::cout << "DETERMINANT:\n" << m3.getDeterminant() << "\n" << std::endl;

	// inverse of non-singluar
	m3.set(1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4);             // non-singular M
	m3.invert();                                            // inverse
	std::cout << "INVERSE:\n" << m3 << std::endl;

	// invert Euclidean (rotation/reflection only)
	m3.set(-1, 0, 0, 0, 0, 0.70711f, 0.70711f, 0, 0, -0.70711f, 0.70711f, 0, 1, 2, 3, 1);
	m3.invertEuclidean();                                   // inverse explicitly
	std::cout << "INVERSE EUCLIDEAN:\n" << m3 << std::endl;

	// translate transform
	m3.identity();
	m3.translate(1, 2, 3);                                  // = glTranslatef(x, y, z)
	//m3.translate(v1);
	std::cout << "TRANSLATE:\n" << m3 << std::endl;

	// rotate transform with degree
	m3.identity();
	m3.rotate(45, 0, 1, 0);                                   // = glRotatef(a, 1,0,0)
	std::cout << "ROTATE:\n" << m3 << std::endl;

	// rotate on basis axis (degree)
	m3.identity();
	m3.rotateX(45);                                         // = glRotatef(45, 1,0,0)
	//m3.rotateY(45);                                         // = glRotatef(45, 0,1,0)
	//m3.rotateZ(45);                                         // = glRotatef(45, 0,0,1)
	std::cout << "ROTATE:\n" << m3 << std::endl;
	std::cout << "ANGLE: " << m3.getAngle() << "\n\n";

	// scale
	m3.identity();
	m3.scale(1, 2, 3);                                      // = glScalef(x, y, z)
	std::cout << "SCALE:\n" << m3 << std::endl;

	// lookat
	m3.identity();
	m3.translate(0, 0, 0);
	m3.lookAt(1, 0, -1);
	std::cout << "LookAt:\n" << m3 << std::endl;
	//=========================================================================

	system("PAUSE");
	return EXIT_SUCCESS;
}