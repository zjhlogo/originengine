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
	x = y = z = w = 0.0f;
}

inline CQuaternion::CQuaternion(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
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

#endif // __QUATERNION_INL__
