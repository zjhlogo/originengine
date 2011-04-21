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

CMatrix4x4 CMatrix4x4::Inverse() const
{
	float fA0 = m[0]*m[5] - m[1]*m[4];
	float fA1 = m[0]*m[6] - m[2]*m[4];
	float fA2 = m[0]*m[7] - m[3]*m[4];
	float fA3 = m[1]*m[6] - m[2]*m[5];
	float fA4 = m[1]*m[7] - m[3]*m[5];
	float fA5 = m[2]*m[7] - m[3]*m[6];

	float fB0 = m[8]*m[13] - m[9]*m[12];
	float fB1 = m[8]*m[14] - m[10]*m[12];
	float fB2 = m[8]*m[15] - m[11]*m[12];
	float fB3 = m[9]*m[14] - m[10]*m[13];
	float fB4 = m[9]*m[15] - m[11]*m[13];
	float fB5 = m[10]*m[15] - m[11]*m[14];

	float fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;

	CMatrix4x4 matInv;
	if (fabs(fDet) <= COEMath::TOL) return matInv;

	matInv.m[0] =  m[5]*fB5 - m[6]*fB4 + m[7]*fB3;
	matInv.m[4] = -m[4]*fB5 + m[6]*fB2 - m[7]*fB1;
	matInv.m[8] =  m[4]*fB4 - m[5]*fB2 + m[7]*fB0;
	matInv.m[12] = -m[4]*fB3 + m[5]*fB1 - m[6]*fB0;

	matInv.m[1] = -m[1]*fB5 + m[2]*fB4 - m[3]*fB3;
	matInv.m[5] =  m[0]*fB5 - m[2]*fB2 + m[3]*fB1;
	matInv.m[9] = -m[0]*fB4 + m[1]*fB2 - m[3]*fB0;
	matInv.m[13] =  m[0]*fB3 - m[1]*fB1 + m[2]*fB0;

	matInv.m[2] =  m[13]*fA5 - m[14]*fA4 + m[15]*fA3;
	matInv.m[6] = -m[12]*fA5 + m[14]*fA2 - m[15]*fA1;
	matInv.m[10] =  m[12]*fA4 - m[13]*fA2 + m[15]*fA0;
	matInv.m[14] = -m[12]*fA3 + m[13]*fA1 - m[14]*fA0;

	matInv.m[3] = -m[9]*fA5 + m[10]*fA4 - m[11]*fA3;
	matInv.m[7] =  m[8]*fA5 - m[10]*fA2 + m[11]*fA1;
	matInv.m[11] = -m[8]*fA4 + m[9]*fA2 - m[11]*fA0;
	matInv.m[15] =  m[8]*fA3 - m[9]*fA1 + m[10]*fA0;

	float fInvDet = 1.0f/fDet;

	matInv.m[0] *= fInvDet;
	matInv.m[1] *= fInvDet;
	matInv.m[2] *= fInvDet;
	matInv.m[3] *= fInvDet;

	matInv.m[4] *= fInvDet;
	matInv.m[5] *= fInvDet;
	matInv.m[6] *= fInvDet;
	matInv.m[7] *= fInvDet;

	matInv.m[8] *= fInvDet;
	matInv.m[9] *= fInvDet;
	matInv.m[10] *= fInvDet;
	matInv.m[11] *= fInvDet;

	matInv.m[12] *= fInvDet;
	matInv.m[13] *= fInvDet;
	matInv.m[14] *= fInvDet;
	matInv.m[15] *= fInvDet;

	return matInv;

}