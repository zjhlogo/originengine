/*!
 * \file OEMath.inl
 * \date 28-5-2009 20:35:55
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMATH_INL__
#define __OEMATH_INL__

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

inline CVector4 operator *(const CVector4& v, float s)
{
	return CVector4(v.x*s, v.y*s, v.z*s, v.w*s);
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
