/*!
 * \file Vector3.inl
 * \date 26-5-2009 21:14:31
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR3_INL__
#define __VECTOR3_INL__

#include <math.h>

inline CVector3::CVector3()
{
	x = y = z = 0.0f;
}

inline CVector3::CVector3(float fx, float fy, float fz)
{
	x = fx;
	y = fy;
	z = fz;
}

inline CVector3::~CVector3()
{
	// TODO: 
}

inline void CVector3::Init(float fx, float fy, float fz)
{
	x = fx;
	y = fy;
	z = fz;
}

inline float CVector3::Length() const
{
	return sqrtf(x*x+y*y+z*z);
}

inline float CVector3::SquaredLength() const
{
	return x*x+y*y+z*z;
}

inline CVector3& CVector3::operator +=(const CVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline CVector3& CVector3::operator -=(const CVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline CVector3& CVector3::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline CVector3& CVector3::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline CVector3 CVector3::operator -() const
{
	return CVector3(-x, -y, -z);
}

inline CVector3 operator +(const CVector3& v1, const CVector3& v2)
{
	return CVector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

inline CVector3 operator -(const CVector3& v1, const CVector3& v2)
{
	return CVector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

inline float operator *(const CVector3& v1, const CVector3& v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

inline CVector3 operator *(float s, const CVector3& v)
{
	return CVector3(v.x*s, v.y*s, v.z*s);
}

inline CVector3 operator *(const CVector3& v, float s)
{
	return CVector3(v.x*s, v.y*s, v.z*s);
}

inline CVector3 operator /(const CVector3& v, float s)
{
	return CVector3(v.x/s, v.y/s, v.z/s);
}

inline CVector3 operator ^(const CVector3& v1, const CVector3& v2)
{
	return CVector3(v1.y*v2.z-v1.z*v2.y, -v1.x*v2.z+v1.z*v2.x, v1.x*v2.y-v1.y*v2.x);
}

#endif // __VECTOR3_INL__
