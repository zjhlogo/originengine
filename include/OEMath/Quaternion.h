/*!
 * \file Quaternion.h
 * \date 26-5-2009 22:22:07
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __QUATERNION_H__
#define __QUATERNION_H__

class CQuaternion
{
public:
	CQuaternion();
	CQuaternion(float fx, float fy, float fz, float fw);

	float Length() const;
	float SquaredLength() const;
	void Normalize();
	CQuaternion Inverse() const;
	float GetRadian() const;

	CQuaternion& operator +=(const CQuaternion& q);
	CQuaternion& operator -=(const CQuaternion& q);
	CQuaternion& operator *=(float s);
	CQuaternion& operator /=(float s);

public:
	float x;
	float y;
	float z;
	float w;	// number (scalar) part

};

#endif // __QUATERNION_H__
