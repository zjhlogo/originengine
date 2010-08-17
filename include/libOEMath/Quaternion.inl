/*!
 * \file Quaternion.inl
 * \date 26-5-2009 21:14:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __QUATERNION_INL__
#define __QUATERNION_INL__

#include <math.h>

inline CQuaternion::CQuaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

inline CQuaternion::CQuaternion(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline CQuaternion::CQuaternion(const CVector3& vDirNormalized, float fRadian)
{
	float fHalfRadian = fRadian/2.0f;
	float fSinTheta = sinf(fHalfRadian);

	x = vDirNormalized.x*fSinTheta;
	y = vDirNormalized.y*fSinTheta;
	z = vDirNormalized.z*fSinTheta;
	w = cosf(fHalfRadian);
}

inline void CQuaternion::Init(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline void CQuaternion::Identity()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

inline float CQuaternion::Length() const
{
	return sqrtf(x*x+y*y+z*z+w*w);
}

inline float CQuaternion::SquaredLength() const
{
	return (x*x+y*y+z*z+w*w);
}

inline float CQuaternion::GetRadian() const
{
	return (2*acosf(w));
}

inline CQuaternion& CQuaternion::operator +=(const CQuaternion& q)
{
	x += q.x;
	y += q.y;
	z += q.z;
	w += q.w;
	return *this;
}

inline CQuaternion& CQuaternion::operator -=(const CQuaternion& q)
{
	x -= q.x;
	y -= q.y;
	z -= q.z;
	w -= q.w;
	return *this;
}

inline CQuaternion& CQuaternion::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

inline CQuaternion& CQuaternion::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

inline CQuaternion CQuaternion::operator -() const
{
	return CQuaternion(-x, -y, -z, w);
}

inline CQuaternion operator +(const CQuaternion& q1, const CQuaternion& q2)
{
	return CQuaternion(q1.x+q2.x, q1.y+q2.y, q1.z+q2.z, q1.w+q2.w);
}

inline CQuaternion operator -(const CQuaternion& q1, const CQuaternion& q2)
{
	return CQuaternion(q1.x-q2.x, q1.y-q2.y, q1.z-q2.z, q1.w-q2.w);
}

inline CQuaternion operator *(const CQuaternion& q1, const CQuaternion& q2)
{
	float prd_0 = (q1.z - q1.y) * (q2.y - q2.z);
	float prd_1 = (q1.w + q1.x) * (q2.w + q2.x);
	float prd_2 = (q1.w - q1.x) * (q2.y + q2.z);
	float prd_3 = (q1.y + q1.z) * (q2.w - q2.x);
	float prd_4 = (q1.z - q1.x) * (q2.x - q2.y);
	float prd_5 = (q1.z + q1.x) * (q2.x + q2.y);
	float prd_6 = (q1.w + q1.y) * (q2.w - q2.z);
	float prd_7 = (q1.w - q1.y) * (q2.w + q2.z);

	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5f * (prd_4 + prd_8);

	return CQuaternion(prd_1 + prd_9 - prd_8,
					   prd_2 + prd_9 - prd_7,
					   prd_3 + prd_9 - prd_6,
					   prd_0 + prd_9 - prd_5);
}

inline CQuaternion operator *(const CQuaternion& q, float s)
{
	return CQuaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

inline CQuaternion operator *(float s, const CQuaternion& q)
{
	return CQuaternion(q.x*s, q.y*s, q.z*s, q.w*s);
}

inline CQuaternion operator *(const CQuaternion& q, const CVector3& v)
{
	return CQuaternion(q.w*v.x + q.y*v.z - q.z*v.y,
					   q.w*v.y + q.z*v.x - q.x*v.z,
					   q.w*v.z + q.x*v.y - q.y*v.x,
					 -(q.x*v.x + q.y*v.y + q.z*v.z));
}

inline CQuaternion operator /(const CQuaternion& q, float s)
{
	return CQuaternion(q.x/s, q.y/s, q.z/s, q.w/s);
}

inline CQuaternion operator *(const CVector3& v, const CQuaternion& q)
{
	return CQuaternion(q.w*v.x + q.z*v.y - q.y*v.z,
					   q.w*v.y + q.x*v.z - q.z*v.x,
					   q.w*v.z + q.y*v.x - q.x*v.y,
					 -(q.x*v.x + q.y*v.y + q.z*v.z));
}

#endif // __QUATERNION_INL__
