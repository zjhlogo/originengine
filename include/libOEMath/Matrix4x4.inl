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
	e[0]  = 1.0f; e[1]  = 0.0f; e[2]  = 0.0f; e[3]  = 0.0f;
	e[4]  = 0.0f; e[5]  = 1.0f; e[6]  = 0.0f; e[7]  = 0.0f;
	e[8]  = 0.0f; e[9]  = 0.0f; e[10] = 1.0f; e[11] = 0.0f;
	e[12] = 0.0f; e[13] = 0.0f; e[14] = 0.0f; e[15] = 1.0f;
}

inline CMatrix4x4::CMatrix4x4(float e00, float e01, float e02, float e03,
							  float e10, float e11, float e12, float e13,
							  float e20, float e21, float e22, float e23, 
							  float e30, float e31, float e32, float e33)
{
	e[0]  = e00; e[1]  = e01; e[2]  = e02; e[3]  = e03;
	e[4]  = e10; e[5]  = e11; e[6]  = e12; e[7]  = e13;
	e[8]  = e20; e[9]  = e21; e[10] = e22; e[11] = e23;
	e[12] = e30; e[13] = e31; e[14] = e32; e[15] = e33;
}

inline void CMatrix4x4::Identity()
{
	e[0]  = 1.0f; e[1]  = 0.0f; e[2]  = 0.0f; e[3]  = 0.0f;
	e[4]  = 0.0f; e[5]  = 1.0f; e[6]  = 0.0f; e[7]  = 0.0f;
	e[8]  = 0.0f; e[9]  = 0.0f; e[10] = 1.0f; e[11] = 0.0f;
	e[12] = 0.0f; e[13] = 0.0f; e[14] = 0.0f; e[15] = 1.0f;
}

inline float CMatrix4x4::Det() const
{
	float fA0 = e[0] * e[5] - e[1] * e[4];
	float fA1 = e[0] * e[6] - e[2] * e[4];
	float fA2 = e[0] * e[7] - e[3] * e[4];
	float fA3 = e[1] * e[6] - e[2] * e[5];
	float fA4 = e[1] * e[7] - e[3] * e[5];
	float fA5 = e[2] * e[7] - e[3] * e[6];

	float fB0 = e[8]  * e[13] - e[9]  * e[12];
	float fB1 = e[8]  * e[14] - e[10] * e[12];
	float fB2 = e[8]  * e[15] - e[11] * e[12];
	float fB3 = e[9]  * e[14] - e[10] * e[13];
	float fB4 = e[9]  * e[15] - e[11] * e[13];
	float fB5 = e[10] * e[15] - e[11] * e[14];

	return fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
}

inline CMatrix4x4& CMatrix4x4::operator +=(const CMatrix4x4& mat)
{
	e[0]  += mat.e[0];  e[1]  += mat.e[1];  e[2]  += mat.e[2];  e[3]  += mat.e[3];
	e[4]  += mat.e[4];  e[5]  += mat.e[5];  e[6]  += mat.e[6];  e[7]  += mat.e[7];
	e[8]  += mat.e[8];  e[9]  += mat.e[9];  e[10] += mat.e[10]; e[11] += mat.e[11];
	e[12] += mat.e[12]; e[13] += mat.e[13]; e[14] += mat.e[14]; e[15] += mat.e[15];
	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator -=(const CMatrix4x4& mat)
{
	e[0]  -= mat.e[0];  e[1]  -= mat.e[1];  e[2]  -= mat.e[2];  e[3]  -= mat.e[3];
	e[4]  -= mat.e[4];  e[5]  -= mat.e[5];  e[6]  -= mat.e[6];  e[7]  -= mat.e[7];
	e[8]  -= mat.e[8];  e[9]  -= mat.e[9];  e[10] -= mat.e[10]; e[11] -= mat.e[11];
	e[12] -= mat.e[12]; e[13] -= mat.e[13]; e[14] -= mat.e[14]; e[15] -= mat.e[15];
	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator *=(float s)
{
	e[0]  *= s; e[1]  *= s; e[2]  *= s; e[3]  *= s;
	e[4]  *= s; e[5]  *= s; e[6]  *= s; e[7]  *= s;
	e[8]  *= s; e[9]  *= s; e[10] *= s; e[11] *= s;
	e[12] *= s; e[13] *= s; e[14] *= s; e[15] *= s;
	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator *=(const CMatrix4x4& mat)
{
	CMatrix4x4 tmpMat(*this);

	e[0] = tmpMat.e[0]*mat.e[0] + tmpMat.e[1]*mat.e[4] + tmpMat.e[2]*mat.e[8] + tmpMat.e[3]*mat.e[12];
	e[1] = tmpMat.e[0]*mat.e[1] + tmpMat.e[1]*mat.e[5] + tmpMat.e[2]*mat.e[9] + tmpMat.e[3]*mat.e[13];
	e[2] = tmpMat.e[0]*mat.e[2] + tmpMat.e[1]*mat.e[6] + tmpMat.e[2]*mat.e[10] + tmpMat.e[3]*mat.e[14];
	e[3] = tmpMat.e[0]*mat.e[3] + tmpMat.e[1]*mat.e[7] + tmpMat.e[2]*mat.e[11] + tmpMat.e[3]*mat.e[15];

	e[4] = tmpMat.e[4]*mat.e[0] + tmpMat.e[5]*mat.e[4] + tmpMat.e[6]*mat.e[8] + tmpMat.e[7]*mat.e[12];
	e[5] = tmpMat.e[4]*mat.e[1] + tmpMat.e[5]*mat.e[5] + tmpMat.e[6]*mat.e[9] + tmpMat.e[7]*mat.e[13];
	e[6] = tmpMat.e[4]*mat.e[2] + tmpMat.e[5]*mat.e[6] + tmpMat.e[6]*mat.e[10] + tmpMat.e[7]*mat.e[14];
	e[7] = tmpMat.e[4]*mat.e[3] + tmpMat.e[5]*mat.e[7] + tmpMat.e[6]*mat.e[11] + tmpMat.e[7]*mat.e[15];

	e[8] = tmpMat.e[8]*mat.e[0] + tmpMat.e[9]*mat.e[4] + tmpMat.e[10]*mat.e[8] + tmpMat.e[11]*mat.e[12];
	e[9] = tmpMat.e[8]*mat.e[1] + tmpMat.e[9]*mat.e[5] + tmpMat.e[10]*mat.e[9] + tmpMat.e[11]*mat.e[13];
	e[10] = tmpMat.e[8]*mat.e[2] + tmpMat.e[9]*mat.e[6] + tmpMat.e[10]*mat.e[10] + tmpMat.e[11]*mat.e[14];
	e[11] = tmpMat.e[8]*mat.e[3] + tmpMat.e[9]*mat.e[7] + tmpMat.e[10]*mat.e[11] + tmpMat.e[11]*mat.e[15];

	e[12] = tmpMat.e[12]*mat.e[0] + tmpMat.e[13]*mat.e[4] + tmpMat.e[14]*mat.e[8] + tmpMat.e[15]*mat.e[12];
	e[13] = tmpMat.e[12]*mat.e[1] + tmpMat.e[13]*mat.e[5] + tmpMat.e[14]*mat.e[9] + tmpMat.e[15]*mat.e[13];
	e[14] = tmpMat.e[12]*mat.e[2] + tmpMat.e[13]*mat.e[6] + tmpMat.e[14]*mat.e[10] + tmpMat.e[15]*mat.e[14];
	e[15] = tmpMat.e[12]*mat.e[3] + tmpMat.e[13]*mat.e[7] + tmpMat.e[14]*mat.e[11] + tmpMat.e[15]*mat.e[15];

	return *this;
}

inline CMatrix4x4& CMatrix4x4::operator /=(float s)
{
	e[0]  /= s; e[1]  /= s; e[2]  /= s; e[3]  /= s;
	e[4]  /= s; e[5]  /= s; e[6]  /= s; e[7]  /= s;
	e[8]  /= s; e[9]  /= s; e[10] /= s; e[11] /= s;
	e[12] /= s; e[13] /= s; e[14] /= s; e[15] /= s;
	return *this;
}

#endif // __MATRIX4X4_INL__
