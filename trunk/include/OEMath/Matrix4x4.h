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
	CMatrix4x4(float fm00, float fm01, float fm02, float fm03,
			   float fm10, float fm11, float fm12, float fm13,
			   float fm20, float fm21, float fm22, float fm23,
			   float fm30, float fm31, float fm32, float fm33);

	void Identity();
	float Det() const;
	CMatrix4x4 Transpose() const;
	CMatrix4x4 Inverse() const;

	CMatrix4x4& operator +=(const CMatrix4x4& mat);
	CMatrix4x4& operator -=(const CMatrix4x4& mat);
	CMatrix4x4& operator *=(float s);
	CMatrix4x4& operator /=(float s);

public:
	float m[16];

};

#include "Matrix4x4.inl"

#endif // __MATRIX4X4_H__
