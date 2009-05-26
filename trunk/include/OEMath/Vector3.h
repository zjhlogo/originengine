/*!
 * \file Vector3.h
 * \date 26-5-2009 21:09:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __VECTOR3_H__
#define __VECTOR3_H__

class CVector3
{
public:
	CVector3();
	CVector3(float fx, float fy, float fz);
	~CVector3();

	float Length() const;
	float SquaredLength() const;
	void Normalize();
	void Reverse();

	CVector3& operator +=(const CVector3& v);
	CVector3& operator -=(const CVector3& v);
	CVector3& operator *=(float s);
	CVector3& operator /=(float s);

	CVector3 operator -() const;

public:
	float x;
	float y;
	float z;

};

#include "Vector3.inl"

#endif // __VECTOR3_H__
