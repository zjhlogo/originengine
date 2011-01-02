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
	return CVector3(v.x*mat.e[0]+v.y*mat.e[4]+v.z*mat.e[8]+mat.e[12],
					v.x*mat.e[1]+v.y*mat.e[5]+v.z*mat.e[9]+mat.e[13],
					v.x*mat.e[2]+v.y*mat.e[6]+v.z*mat.e[10]+mat.e[14]);
}

inline CVector3 operator *(const CMatrix4x4& mat, const CVector3& v)
{
	return CVector3(v.x*mat.e[0]+v.y*mat.e[4]+v.z*mat.e[8]+mat.e[12],
					v.x*mat.e[1]+v.y*mat.e[5]+v.z*mat.e[9]+mat.e[13],
					v.x*mat.e[2]+v.y*mat.e[6]+v.z*mat.e[10]+mat.e[14]);
}

inline CVector4 operator *(const CVector4& v, const CMatrix4x4& mat)
{
	return CVector4(v.x*mat.e[0]+v.y*mat.e[4]+v.z*mat.e[8]+v.w*mat.e[12],
					v.x*mat.e[1]+v.y*mat.e[5]+v.z*mat.e[9]+v.w*mat.e[13],
					v.x*mat.e[2]+v.y*mat.e[6]+v.z*mat.e[10]+v.w*mat.e[14],
					v.x*mat.e[3]+v.y*mat.e[7]+v.z*mat.e[11]+v.w*mat.e[15]);
}

inline CVector4 operator *(const CMatrix4x4& mat, const CVector4& v)
{
	return CVector4(v.x*mat.e[0]+v.y*mat.e[4]+v.z*mat.e[8]+v.w*mat.e[12],
					v.x*mat.e[1]+v.y*mat.e[5]+v.z*mat.e[9]+v.w*mat.e[13],
					v.x*mat.e[2]+v.y*mat.e[6]+v.z*mat.e[10]+v.w*mat.e[14],
					v.x*mat.e[3]+v.y*mat.e[7]+v.z*mat.e[11]+v.w*mat.e[15]);
}

inline CMatrix4x4 operator *(const CMatrix4x4& m1, const CMatrix4x4& m2)
{
	return CMatrix4x4(m1.e[0]*m2.e[0] + m1.e[1]*m2.e[4] + m1.e[2]*m2.e[8] + m1.e[3]*m2.e[12],
		m1.e[0]*m2.e[1] + m1.e[1]*m2.e[5] + m1.e[2]*m2.e[9] + m1.e[3]*m2.e[13],
		m1.e[0]*m2.e[2] + m1.e[1]*m2.e[6] + m1.e[2]*m2.e[10] + m1.e[3]*m2.e[14],
		m1.e[0]*m2.e[3] + m1.e[1]*m2.e[7] + m1.e[2]*m2.e[11] + m1.e[3]*m2.e[15],

		m1.e[4]*m2.e[0] + m1.e[5]*m2.e[4] + m1.e[6]*m2.e[8] + m1.e[7]*m2.e[12],
		m1.e[4]*m2.e[1] + m1.e[5]*m2.e[5] + m1.e[6]*m2.e[9] + m1.e[7]*m2.e[13],
		m1.e[4]*m2.e[2] + m1.e[5]*m2.e[6] + m1.e[6]*m2.e[10] + m1.e[7]*m2.e[14],
		m1.e[4]*m2.e[3] + m1.e[5]*m2.e[7] + m1.e[6]*m2.e[11] + m1.e[7]*m2.e[15],

		m1.e[8]*m2.e[0] + m1.e[9]*m2.e[4] + m1.e[10]*m2.e[8] + m1.e[11]*m2.e[12],
		m1.e[8]*m2.e[1] + m1.e[9]*m2.e[5] + m1.e[10]*m2.e[9] + m1.e[11]*m2.e[13],
		m1.e[8]*m2.e[2] + m1.e[9]*m2.e[6] + m1.e[10]*m2.e[10] + m1.e[11]*m2.e[14],
		m1.e[8]*m2.e[3] + m1.e[9]*m2.e[7] + m1.e[10]*m2.e[11] + m1.e[11]*m2.e[15],

		m1.e[12]*m2.e[0] + m1.e[13]*m2.e[4] + m1.e[14]*m2.e[8] + m1.e[15]*m2.e[12],
		m1.e[12]*m2.e[1] + m1.e[13]*m2.e[5] + m1.e[14]*m2.e[9] + m1.e[15]*m2.e[13],
		m1.e[12]*m2.e[2] + m1.e[13]*m2.e[6] + m1.e[14]*m2.e[10] + m1.e[15]*m2.e[14],
		m1.e[12]*m2.e[3] + m1.e[13]*m2.e[7] + m1.e[14]*m2.e[11] + m1.e[15]*m2.e[15]);
}

inline void COEMath::SetMatrixTranslation(CMatrix4x4& matOut, const CVector3& vPos)
{
	matOut.e[12] = vPos.x;
	matOut.e[13] = vPos.y;
	matOut.e[14] = vPos.z;
}

inline void COEMath::GetMatrixTranslation(CVector3& vPosOut, const CMatrix4x4& matIn)
{
	vPosOut.x = matIn.e[12];
	vPosOut.y = matIn.e[13];
	vPosOut.z = matIn.e[14];
}

inline void COEMath::SetMatrixScale(CMatrix4x4& matOut, const CVector3& vScale)
{
	matOut.e[0] *= vScale.x;
	matOut.e[1] *= vScale.x;
	matOut.e[2] *= vScale.x;

	matOut.e[4] *= vScale.y;
	matOut.e[5] *= vScale.y;
	matOut.e[6] *= vScale.y;

	matOut.e[8] *= vScale.z;
	matOut.e[9] *= vScale.z;
	matOut.e[10] *= vScale.z;
}

inline void COEMath::GetMatrixScale(CVector3& vScaleOut, const CMatrix4x4& matIn)
{
	vScaleOut.x = sqrtf(matIn.e[0]*matIn.e[0] + matIn.e[1]*matIn.e[1] + matIn.e[2]*matIn.e[2]);
	vScaleOut.y = sqrtf(matIn.e[4]*matIn.e[4] + matIn.e[5]*matIn.e[5] + matIn.e[6]*matIn.e[6]);
	vScaleOut.z = sqrtf(matIn.e[8]*matIn.e[8] + matIn.e[9]*matIn.e[9] + matIn.e[10]*matIn.e[10]);
}

inline void COEMath::InverseTranslateVector(CVector3& vInOut, const CMatrix4x4& matIn)
{
	vInOut.x = vInOut.x - matIn.e[12];
	vInOut.y = vInOut.y - matIn.e[13];
	vInOut.z = vInOut.z - matIn.e[14];
}

inline void COEMath::InverseRotateVector(CVector3& vInOut, const CMatrix4x4& matIn)
{
	CVector3 vTemp;
	vTemp.x = vInOut.x*matIn.e[0] + vInOut.y*matIn.e[1] + vInOut.z*matIn.e[2];
	vTemp.y = vInOut.x*matIn.e[4] + vInOut.y*matIn.e[5] + vInOut.z*matIn.e[6];
	vTemp.z = vInOut.x*matIn.e[8] + vInOut.y*matIn.e[9] + vInOut.z*matIn.e[10];

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
