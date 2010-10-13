/*!
 * \file Camera.h
 * \date 28-5-2009 21:13:35
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <libOEMath/OEMath.h>

class CCamera
{
public:
	CCamera();
	~CCamera();

	void Initialize(const CVector3& vEye, const CVector3& vLookAt);
	void InitFromBBox(const CVector3& vMinBBox, const CVector3& vMaxBBox);

	const CMatrix4x4& GetViewMatrix() const;

	void Rotate(float fRotY, float fRotX);
	void Move(const CVector3& vDir, float fDistance);

	const CVector3& GetEyePos() const;
	const CVector3& GetLookAtPos() const;

	const CVector3& GetVectorUp() const;
	const CVector3& GetVectorForword() const;
	const CVector3& GetVectorRight() const;

private:
	void Init();
	void Destroy();

	void SyncRotFromForward(const CVector3& vForward);

private:
	CVector3 m_vEye;
	CVector3 m_vLookAt;

	CVector3 m_vUp;
	CVector3 m_vForward;
	CVector3 m_vRight;

	float m_fRotX;
	float m_fRotY;

	CMatrix4x4 m_matView;
};
#endif // __CAMERA_H__