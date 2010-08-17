/*!
 * \file Vector4.h
 * \date 26-5-2009 21:27:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR4_H__
#define __VECTOR4_H__

class CVector4
{
public:
	CVector4();
	CVector4(float fx, float fy, float fz, float fw);
	~CVector4();

	void Init(float fx, float fy, float fz, float fw);
	float Length() const;
	float SquaredLength() const;
	void Normalize();

	CVector4& operator +=(const CVector4& v);
	CVector4& operator -=(const CVector4& v);
	CVector4& operator *=(float s);
	CVector4& operator /=(float s);

	CVector4 operator -() const;

public:
	float x;
	float y;
	float z;
	float w;

};

#include "Vector4.inl"

#endif // __VECTOR4_H__
