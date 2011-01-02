/*!
 * \file Matrix4x4.cpp
 * \date 26-5-2009 22:12:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include <libOEMath/Matrix4x4.h>
#include <libOEMath/OEMath.h>
#include <math.h>
#include <cassert>

CMatrix4x4::CMatrix4x4(const CMatrix4x4& mat)
{
	memcpy(e, mat.e, sizeof(e));
}

CMatrix4x4& CMatrix4x4::Transpose()
{
	CMatrix4x4 tmpMat = *this;

	e[0] = tmpMat.e[0];
	e[1] = tmpMat.e[4];
	e[2] = tmpMat.e[8];
	e[3] = tmpMat.e[12];

	e[4] = tmpMat.e[1];
	e[5] = tmpMat.e[5];
	e[6] = tmpMat.e[9];
	e[7] = tmpMat.e[13];

	e[8] = tmpMat.e[2];
	e[9] = tmpMat.e[6];
	e[10] = tmpMat.e[10];
	e[11] = tmpMat.e[14];

	e[12] = tmpMat.e[3];
	e[13] = tmpMat.e[7];
	e[14] = tmpMat.e[11];
	e[15] = tmpMat.e[15];

	return *this;
}

CMatrix4x4& CMatrix4x4::Inverse()
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

	float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;

	if (fabs(fDet) <= COEMath::TOL)
	{
		assert(false);
		return *this;
	}

	CMatrix4x4 tmpMat(*this);

	e[0] =  tmpMat.e[5]*fB5 - tmpMat.e[6]*fB4 + tmpMat.e[7]*fB3;
	e[4] = -tmpMat.e[4]*fB5 + tmpMat.e[6]*fB2 - tmpMat.e[7]*fB1;
	e[8] =  tmpMat.e[4]*fB4 - tmpMat.e[5]*fB2 + tmpMat.e[7]*fB0;
	e[12] = -tmpMat.e[4]*fB3 + tmpMat.e[5]*fB1 - tmpMat.e[6]*fB0;

	e[1] = -tmpMat.e[1]*fB5 + tmpMat.e[2]*fB4 - tmpMat.e[3]*fB3;
	e[5] =  tmpMat.e[0]*fB5 - tmpMat.e[2]*fB2 + tmpMat.e[3]*fB1;
	e[9] = -tmpMat.e[0]*fB4 + tmpMat.e[1]*fB2 - tmpMat.e[3]*fB0;
	e[13] =  tmpMat.e[0]*fB3 - tmpMat.e[1]*fB1 + tmpMat.e[2]*fB0;

	e[2] =  tmpMat.e[13]*fA5 - tmpMat.e[14]*fA4 + tmpMat.e[15]*fA3;
	e[6] = -tmpMat.e[12]*fA5 + tmpMat.e[14]*fA2 - tmpMat.e[15]*fA1;
	e[10] =  tmpMat.e[12]*fA4 - tmpMat.e[13]*fA2 + tmpMat.e[15]*fA0;
	e[14] = -tmpMat.e[12]*fA3 + tmpMat.e[13]*fA1 - tmpMat.e[14]*fA0;

	e[3] = -tmpMat.e[9]*fA5 + tmpMat.e[10]*fA4 - tmpMat.e[11]*fA3;
	e[7] =  tmpMat.e[8]*fA5 - tmpMat.e[10]*fA2 + tmpMat.e[11]*fA1;
	e[11] = -tmpMat.e[8]*fA4 + tmpMat.e[9]*fA2 - tmpMat.e[11]*fA0;
	e[15] =  tmpMat.e[8]*fA3 - tmpMat.e[9]*fA1 + tmpMat.e[10]*fA0;

	float fInvDet = 1.0f/fDet;

	e[0] *= fInvDet;
	e[1] *= fInvDet;
	e[2] *= fInvDet;
	e[3] *= fInvDet;

	e[4] *= fInvDet;
	e[5] *= fInvDet;
	e[6] *= fInvDet;
	e[7] *= fInvDet;

	e[8] *= fInvDet;
	e[9] *= fInvDet;
	e[10] *= fInvDet;
	e[11] *= fInvDet;

	e[12] *= fInvDet;
	e[13] *= fInvDet;
	e[14] *= fInvDet;
	e[15] *= fInvDet;
	return *this;
}
