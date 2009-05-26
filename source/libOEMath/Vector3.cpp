/*!
 * \file Vector3.cpp
 * \date 26-5-2009 21:15:59
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMath/Vector3.h>
#include <OEMath/OEMath.h>

void CVector3::Normalize()
{
	float m = Length();
	if (m <= OEMATH_TOL) m = 1.0f;

	x /= m;
	y /= m;
	z /= m;

	if (fabsf(x) < OEMATH_TOL) x = 0.0f;
	if (fabsf(y) < OEMATH_TOL) y = 0.0f;
	if (fabsf(z) < OEMATH_TOL) z = 0.0f;
}
