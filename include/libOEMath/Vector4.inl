/*!
 * \file Vector4.inl
 * \date 26-5-2009 21:13:48
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR4_INL__
#define __VECTOR4_INL__

#include <math.h>

inline CVector4::CVector4()
{
	x = y = z = w = 0.0f;
}

inline CVector4::CVector4(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline CVector4::~CVector4()
{
	// TODO: 
}

inline void CVector4::Init(float fx, float fy, float fz, float fw)
{
	x = fx;
	y = fy;
	z = fz;
	w = fw;
}

inline float CVector4::Length() const
{
	return sqrtf(x*x+y*y+z*z+w*w);
}

inline float CVector4::SquaredLength() const
{
	return x*x+y*y+z*z+w*w;
}

inline CVector4& CVector4::operator +=(const CVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

inline CVector4& CVector4::operator -=(const CVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

inline CVector4& CVector4::operator *=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

inline CVector4& CVector4::operator /=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

inline CVector4 CVector4::operator -() const
{
	return CVector4(-x, -y, -z, -w);
}

inline CVector4 operator *(const CVector4& v, float s)
{
	return CVector4(v.x*s, v.y*s, v.z*s, v.w*s);
}

inline CVector4 operator *(float s, const CVector4& v)
{
	return CVector4(v.x*s, v.y*s, v.z*s, v.w*s);
}

#endif // __VECTOR4_INL__
