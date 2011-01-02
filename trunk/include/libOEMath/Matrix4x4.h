/*!
 * \file Matrix4x4.h
 * \date 26-5-2009 21:50:41
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

class CMatrix4x4
{
public:
	CMatrix4x4();
	CMatrix4x4(const CMatrix4x4& mat);
	CMatrix4x4(float e00, float e01, float e02, float e03,
			   float e10, float e11, float e12, float e13,
			   float e20, float e21, float e22, float e23,
			   float e30, float e31, float e32, float e33);

	void Identity();
	float Det() const;
	CMatrix4x4& Transpose();
	CMatrix4x4& Inverse();

	CMatrix4x4& operator +=(const CMatrix4x4& mat);
	CMatrix4x4& operator -=(const CMatrix4x4& mat);
	CMatrix4x4& operator *=(float s);
	CMatrix4x4& operator *=(const CMatrix4x4& mat);
	CMatrix4x4& operator /=(float s);

public:
	float e[16];

};

#include "Matrix4x4.inl"

#endif // __MATRIX4X4_H__
