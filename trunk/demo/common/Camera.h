/*!
 * \file Camera.h
 * \date 28-5-2009 21:13:35
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <OEMath/OEMath.h>

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Initialize(const CVector3& vEye, const CVector3& vLookAt);
	const CMatrix4x4& GetViewMatrix() const;

	void Rotate(const CVector3& vDir, float fRadian);
	void Move(const CVector3& vDir, float fDistance);

	const CVector3& GetEyePos() const;
	const CVector3& GetLookAtPos() const;

	const CVector3& GetVectorUp() const;
	const CVector3& GetVectorForword() const;
	const CVector3& GetVectorRight() const;

private:
	void Init();
	void Destroy();

private:
	CVector3 m_vEye;
	CVector3 m_vLookAt;

	CVector3 m_vUp;
	CVector3 m_vForword;
	CVector3 m_vRight;

	CMatrix4x4 m_matView;
};
#endif // __CAMERA_H__