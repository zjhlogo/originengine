/*!
 * \file Matrix4x4.inl
 * \date 26-5-2009 21:14:14
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MATRIX4X4_INL__
#define __MATRIX4X4_INL__

#include <math.h>

inline CMatrix4x4::CMatrix4x4()
{
	m[0]  = 1.0f; m[1]  = 0.0f; m[2]  = 0.0f; m[3]  = 0.0f;
	m[4]  = 0.0f; m[5]  = 1.0f; m[6]  = 0.0f; m[7]  = 0.0f;
	m[8]  = 0.0f; m[9]  = 0.0f; m[10] = 1.0f; m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
}

inline CMatrix4x4::CMatrix4x4(float fm00, float fm01, float fm02, float fm03,
							  float fm10, float fm11, float fm12, float fm13,
							  float fm20, float fm21, float fm22, float fm23,
							  float fm30, float fm31, float fm32, float fm33)
{
	m[0]  = fm00; m[1]  = fm01; m[2]  = fm02; m[3]  = fm03;
	m[4]  = fm10; m[5]  = fm11; m[6]  = fm12; m[7]  = fm13;
	m[8]  = fm20; m[9]  = fm21; m[10] = fm22; m[11] = fm23;
	m[12] = fm30; m[13] = fm31; m[14] = fm32; m[15] = fm33;
}

inline float CMatrix4x4::Det() const
{
	float fA0 = m[0] * m[5] - m[1] * m[4];
	float fA1 = m[0] * m[6] - m[2] * m[4];
	float fA2 = m[0] * m[7] - m[3] * m[4];
	float fA3 = m[1] * m[6] - m[2] * m[5];
	float fA4 = m[1] * m[7] - m[3] * m[5];
	float fA5 = m[2] * m[7] - m[3] * m[6];

	float fB0 = m[8]  * m[13] - m[9]  * m[12];
	float fB1 = m[8]  * m[14] - m[10] * m[12];
	float fB2 = m[8]  * m[15] - m[11] * m[12];
	float fB3 = m[9]  * m[14] - m[10] * m[13];
	float fB4 = m[9]  * m[15] - m[11] * m[13];
	float fB5 = m[10] * m[15] - m[11] * m[14];

	return fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
}

inline void CMatrix4x4::Identity()
{
	m[0]  = 1.0f; m[1]  = 0.0f; m[2]  = 0.0f; m[3]  = 0.0f;
	m[4]  = 0.0f; m[5]  = 1.0f; m[6]  = 0.0f; m[7]  = 0.0f;
	m[8]  = 0.0f; m[9]  = 0.0f; m[10] = 1.0f; m[11] = 0.0f;
	m[12] = 0.0f; m[13] = 0.0f; m[14] = 0.0f; m[15] = 1.0f;
}

inline CMatrix4x4 CMatrix4x4::Transpose() const
{
	return CMatrix4x4(m[0], m[4], m[8],  m[12],
					  m[1], m[5], m[9],  m[13],
					  m[2], m[6], m[10], m[14],
					  m[3], m[7], m[11], m[15]);
}

inline CMatrix4x4& CMatrix4x4::operator +=(const CMatrix4x4& mat)
{
	m[0]  += mat.m[0];  m[1]  += mat.m[1];  m[2]  += mat.m[2];  m[3]  += mat.m[3];
	m[4]  += mat.m[4];  m[5]  += mat.m[5];  m[6]  += mat.m[6];  m[7]  += mat.m[7];
	m[8]  += mat.m[8];  m[9]  += mat.m[9];  m[10] += mat.m[10]; m[11] += mat.m[11];
	m[12] += mat.m[12]; m[13] += mat.m[13]; m[14] += mat.m[14]; m[15] += mat.m[15];
	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator -=(const CMatrix4x4& mat)
{
	m[0]  -= mat.m[0];  m[1]  -= mat.m[1];  m[2]  -= mat.m[2];  m[3]  -= mat.m[3];
	m[4]  -= mat.m[4];  m[5]  -= mat.m[5];  m[6]  -= mat.m[6];  m[7]  -= mat.m[7];
	m[8]  -= mat.m[8];  m[9]  -= mat.m[9];  m[10] -= mat.m[10]; m[11] -= mat.m[11];
	m[12] -= mat.m[12]; m[13] -= mat.m[13]; m[14] -= mat.m[14]; m[15] -= mat.m[15];
	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator *=(float s)
{
	m[0]  *= s; m[1]  *= s; m[2]  *= s; m[3]  *= s;
	m[4]  *= s; m[5]  *= s; m[6]  *= s; m[7]  *= s;
	m[8]  *= s; m[9]  *= s; m[10] *= s; m[11] *= s;
	m[12] *= s; m[13] *= s; m[14] *= s; m[15] *= s;
	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator /=(float s)
{
	m[0]  /= s; m[1]  /= s; m[2]  /= s; m[3]  /= s;
	m[4]  /= s; m[5]  /= s; m[6]  /= s; m[7]  /= s;
	m[8]  /= s; m[9]  /= s; m[10] /= s; m[11] /= s;
	m[12] /= s; m[13] /= s; m[14] /= s; m[15] /= s;
	return *this;
}

#endif // __MATRIX4X4_INL__
