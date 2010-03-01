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
extern const float OEMATH_2PI;		// 2*pi
extern const float OEMATH_G;		// acceleration due to gravity, ft/s^2
extern const float OEMATH_RHO;		// desity of air at sea level, slugs/ft^3
extern const float OEMATH_TOL;		// float type tolerance

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include <vector>

CMatrix4x4 operator *(const CMatrix4x4& m1, const CMatrix4x4& m2);

CVector3 operator *(const CVector3& v, const CMatrix4x4& mat);
CVector3 operator *(const CMatrix4x4& mat, const CVector3& v);
CVector4 operator *(const CVector4& v, float s);

class COEMath
{
public:
	static void BuildLookAtMatrixLH(CMatrix4x4& matOut, const CVector3& vEye, const CVector3& vAt, const CVector3& vUp);
	static void BuildProjectMatrixLH(CMatrix4x4& matOut, float fFov, float fAspect, float fNear, float fFar);

	static void BuildQuaternionFromMatrix(CQuaternion& qOut, const CMatrix4x4& matIn);
	static void BuildMatrixFromQuaternion(CMatrix4x4& matOut, const CQuaternion& qIn);

	static void BuildMatrixFromEulerXYZ(CMatrix4x4& matOut, float x, float y, float z);
	static void BuildQuaternionFromEulerXYZ(CQuaternion& qOut, float x, float y, float z);

	static void VectorLerp(CVector3& vOut, const CVector3& v1, const CVector3& v2, float t);
	static void MatrixLerp(CMatrix4x4& matOut, const CMatrix4x4& mat1, const CMatrix4x4& mat2, float t);
	static void QuaternionSlerp(CQuaternion& qOut, const CQuaternion& q1, const CQuaternion& q2, float t);

	static void SetMatrixTranslation(CMatrix4x4& matOut, const CVector3& vPos);
	static void GetMatrixTranslation(CVector3& vPosOut, const CMatrix4x4& matIn);

	static void SetMatrixScale(CMatrix4x4& matOut, const CVector3& vScale);
	static void GetMatrixScale(CVector3& vScaleOut, const CMatrix4x4& matIn);

	static void InverseTranslateVector(CVector3& vInOut, const CMatrix4x4& matIn);
	static void InverseRotateVector(CVector3& vInOut, const CMatrix4x4& matIn);

	static float Max(float a, float b);
	static float Min(float a, float b);
};

#include "OEMath.inl"

typedef std::vector<CMatrix4x4> TV_MATRIX4X4;

#endif // __OEMATH_H__
