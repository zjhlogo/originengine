/*!
 * \file Quaternion.cpp
 * \date 26-5-2009 22:29:57
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <OEMath/Quaternion.h>
#include <OEMath/OEMath.h>

void CQuaternion::Normalize()
{
	float m = Length();
	if (m <= OEMATH_TOL) m = 1.0f;

	x /= m;
	y /= m;
	z /= m;
	w /= m;

	if (fabsf(x) < OEMATH_TOL) x = 0.0f;
	if (fabsf(y) < OEMATH_TOL) y = 0.0f;
	if (fabsf(z) < OEMATH_TOL) z = 0.0f;
	if (fabsf(w) < OEMATH_TOL) w = 0.0f;
}

CQuaternion CQuaternion::Inverse() const
{
	float m = SquaredLength();
	if (m <= OEMATH_TOL) m = 1.0f;

	float fx = x/m;
	float fy = y/m;
	float fz = z/m;
	float fw = w/m;

	if (fabsf(fx) < OEMATH_TOL) fx = 0.0f;
	if (fabsf(fy) < OEMATH_TOL) fy = 0.0f;
	if (fabsf(fz) < OEMATH_TOL) fz = 0.0f;
	if (fabsf(fw) < OEMATH_TOL) fw = 0.0f;

	return CQuaternion(fx, fy, fz, fw);
}
