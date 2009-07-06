/*!
 * \file OEMath.h
 * \date 26-5-2009 22:42:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMATH_H__
#define __OEMATH_H__

extern const float OEMATH_PI;		// pi
extern const float OEMATH_G;		// acceleration due to gravity, ft/s^2
extern const float OEMATH_RHO;		// desity of air at sea level, slugs/ft^3
extern const float OEMATH_TOL;		// float type tolerance

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"

// matrix
CMatrix4x4 operator *(const CMatrix4x4& m1, const CMatrix4x4& m2);

// vector
CVector4 operator *(const CVector4& v, float s);

class COEMath
{
public:
	static void BuildLookAtMatrixLH(CMatrix4x4& matOut, const CVector3& vEye, const CVector3& vAt, const CVector3& vUp);
	static void BuildProjectMatrixLH(CMatrix4x4& matOut, float fFov, float fAspect, float fNear, float fFar);

	static float Max(float a, float b);
	static float Min(float a, float b);
};

#include "OEMath.inl"

#endif // __OEMATH_H__