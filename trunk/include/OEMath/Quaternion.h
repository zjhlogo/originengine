/*!
 * \file Quaternion.h
 * \date 26-5-2009 22:22:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "Vector3.h"

class CQuaternion
{
public:
	CQuaternion();
	CQuaternion(float fx, float fy, float fz, float fw);
	CQuaternion(const CVector3& vDirNormalized, float fRadian);

	void Identity();
	float Length() const;
	float SquaredLength() const;
	void Normalize();
	float GetRadian() const;

	CQuaternion& operator +=(const CQuaternion& q);
	CQuaternion& operator -=(const CQuaternion& q);
	CQuaternion& operator *=(float s);
	CQuaternion& operator /=(float s);
	CQuaternion operator -() const;

public:
	float x;
	float y;
	float z;
	float w;	// number (scalar) part

};

CQuaternion operator +(const CQuaternion& q1, const CQuaternion& q2);
CQuaternion operator -(const CQuaternion& q1, const CQuaternion& q2);
CQuaternion operator *(const CQuaternion& q1, const CQuaternion& q2);
CQuaternion operator *(const CQuaternion& q, float s);
CQuaternion operator *(float s, const CQuaternion& q);
CQuaternion operator /(const CQuaternion& q, float s);
CQuaternion operator *(const CQuaternion& q, const CVector3& v);
CQuaternion operator *(const CVector3& v, const CQuaternion& q);

#include "Quaternion.inl"

#endif // __QUATERNION_H__
