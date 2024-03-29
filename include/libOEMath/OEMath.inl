/*!
 * \file OEMath.inl
 * \date 28-5-2009 20:35:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMATH_INL__
#define __OEMATH_INL__

inline CVector3 operator *(const CVector3& v, const CMatrix4x4& mat)
{
	return CVector3(v.x*mat.m[0]+v.y*mat.m[4]+v.z*mat.m[8]+mat.m[12],
					v.x*mat.m[1]+v.y*mat.m[5]+v.z*mat.m[9]+mat.m[13],
					v.x*mat.m[2]+v.y*mat.m[6]+v.z*mat.m[10]+mat.m[14]);
}

inline CVector3 operator *(const CMatrix4x4& mat, const CVector3& v)
{
	return CVector3(v.x*mat.m[0]+v.y*mat.m[4]+v.z*mat.m[8]+mat.m[12],
					v.x*mat.m[1]+v.y*mat.m[5]+v.z*mat.m[9]+mat.m[13],
					v.x*mat.m[2]+v.y*mat.m[6]+v.z*mat.m[10]+mat.m[14]);
}

inline CVector4 operator *(const CVector4& v, const CMatrix4x4& mat)
{
	return CVector4(v.x*mat.m[0]+v.y*mat.m[4]+v.z*mat.m[8]+v.w*mat.m[12],
					v.x*mat.m[1]+v.y*mat.m[5]+v.z*mat.m[9]+v.w*mat.m[13],
					v.x*mat.m[2]+v.y*mat.m[6]+v.z*mat.m[10]+v.w*mat.m[14],
					v.x*mat.m[3]+v.y*mat.m[7]+v.z*mat.m[11]+v.w*mat.m[15]);
}

inline CVector4 operator *(const CMatrix4x4& mat, const CVector4& v)
{
	return CVector4(v.x*mat.m[0]+v.y*mat.m[4]+v.z*mat.m[8]+v.w*mat.m[12],
					v.x*mat.m[1]+v.y*mat.m[5]+v.z*mat.m[9]+v.w*mat.m[13],
					v.x*mat.m[2]+v.y*mat.m[6]+v.z*mat.m[10]+v.w*mat.m[14],
					v.x*mat.m[3]+v.y*mat.m[7]+v.z*mat.m[11]+v.w*mat.m[15]);
}

inline CMatrix4x4 operator *(const CMatrix4x4& m1, const CMatrix4x4& m2)
{
	return CMatrix4x4(m1.m[0]*m2.m[0] + m1.m[1]*m2.m[4] + m1.m[2]*m2.m[8] + m1.m[3]*m2.m[12],
		m1.m[0]*m2.m[1] + m1.m[1]*m2.m[5] + m1.m[2]*m2.m[9] + m1.m[3]*m2.m[13],
		m1.m[0]*m2.m[2] + m1.m[1]*m2.m[6] + m1.m[2]*m2.m[10] + m1.m[3]*m2.m[14],
		m1.m[0]*m2.m[3] + m1.m[1]*m2.m[7] + m1.m[2]*m2.m[11] + m1.m[3]*m2.m[15],

		m1.m[4]*m2.m[0] + m1.m[5]*m2.m[4] + m1.m[6]*m2.m[8] + m1.m[7]*m2.m[12],
		m1.m[4]*m2.m[1] + m1.m[5]*m2.m[5] + m1.m[6]*m2.m[9] + m1.m[7]*m2.m[13],
		m1.m[4]*m2.m[2] + m1.m[5]*m2.m[6] + m1.m[6]*m2.m[10] + m1.m[7]*m2.m[14],
		m1.m[4]*m2.m[3] + m1.m[5]*m2.m[7] + m1.m[6]*m2.m[11] + m1.m[7]*m2.m[15],

		m1.m[8]*m2.m[0] + m1.m[9]*m2.m[4] + m1.m[10]*m2.m[8] + m1.m[11]*m2.m[12],
		m1.m[8]*m2.m[1] + m1.m[9]*m2.m[5] + m1.m[10]*m2.m[9] + m1.m[11]*m2.m[13],
		m1.m[8]*m2.m[2] + m1.m[9]*m2.m[6] + m1.m[10]*m2.m[10] + m1.m[11]*m2.m[14],
		m1.m[8]*m2.m[3] + m1.m[9]*m2.m[7] + m1.m[10]*m2.m[11] + m1.m[11]*m2.m[15],

		m1.m[12]*m2.m[0] + m1.m[13]*m2.m[4] + m1.m[14]*m2.m[8] + m1.m[15]*m2.m[12],
		m1.m[12]*m2.m[1] + m1.m[13]*m2.m[5] + m1.m[14]*m2.m[9] + m1.m[15]*m2.m[13],
		m1.m[12]*m2.m[2] + m1.m[13]*m2.m[6] + m1.m[14]*m2.m[10] + m1.m[15]*m2.m[14],
		m1.m[12]*m2.m[3] + m1.m[13]*m2.m[7] + m1.m[14]*m2.m[11] + m1.m[15]*m2.m[15]);
}

inline void COEMath::SetMatrixTranslation(CMatrix4x4& matOut, const CVector3& vPos)
{
	matOut.m[12] = vPos.x;
	matOut.m[13] = vPos.y;
	matOut.m[14] = vPos.z;
}

inline void COEMath::GetMatrixTranslation(CVector3& vPosOut, const CMatrix4x4& matIn)
{
	vPosOut.x = matIn.m[12];
	vPosOut.y = matIn.m[13];
	vPosOut.z = matIn.m[14];
}

inline void COEMath::SetMatrixScale(CMatrix4x4& matOut, const CVector3& vScale)
{
	matOut.m[0] *= vScale.x;
	matOut.m[1] *= vScale.x;
	matOut.m[2] *= vScale.x;

	matOut.m[4] *= vScale.y;
	matOut.m[5] *= vScale.y;
	matOut.m[6] *= vScale.y;

	matOut.m[8] *= vScale.z;
	matOut.m[9] *= vScale.z;
	matOut.m[10] *= vScale.z;
}

inline void COEMath::GetMatrixScale(CVector3& vScaleOut, const CMatrix4x4& matIn)
{
	vScaleOut.x = sqrtf(matIn.m[0]*matIn.m[0] + matIn.m[1]*matIn.m[1] + matIn.m[2]*matIn.m[2]);
	vScaleOut.y = sqrtf(matIn.m[4]*matIn.m[4] + matIn.m[5]*matIn.m[5] + matIn.m[6]*matIn.m[6]);
	vScaleOut.z = sqrtf(matIn.m[8]*matIn.m[8] + matIn.m[9]*matIn.m[9] + matIn.m[10]*matIn.m[10]);
}

inline void COEMath::InverseTranslateVector(CVector3& vInOut, const CMatrix4x4& matIn)
{
	vInOut.x = vInOut.x - matIn.m[12];
	vInOut.y = vInOut.y - matIn.m[13];
	vInOut.z = vInOut.z - matIn.m[14];
}

inline void COEMath::InverseRotateVector(CVector3& vInOut, const CMatrix4x4& matIn)
{
	CVector3 vTemp;
	vTemp.x = vInOut.x*matIn.m[0] + vInOut.y*matIn.m[1] + vInOut.z*matIn.m[2];
	vTemp.y = vInOut.x*matIn.m[4] + vInOut.y*matIn.m[5] + vInOut.z*matIn.m[6];
	vTemp.z = vInOut.x*matIn.m[8] + vInOut.y*matIn.m[9] + vInOut.z*matIn.m[10];

	vInOut = vTemp;
}

inline float COEMath::Max(float a, float b)
{
	return a > b ? a : b;
}

inline float COEMath::Min(float a, float b)
{
	return a < b ? a : b;
}

#endif // __OEMATH_INL__
