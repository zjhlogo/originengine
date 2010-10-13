/*!
 * \file Camera.cpp
 * \date 28-5-2009 21:13:45
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "Camera.h"

CCamera::CCamera()
{
	Init();
}

CCamera::~CCamera()
{
	Destroy();
}

void CCamera::Init()
{
	m_fRotX = 0.0f;
	m_fRotY = 0.0f;
	Rotate(0.0f, 0.0f);
}

void CCamera::Destroy()
{
	// TODO: 
}

void CCamera::Initialize(const CVector3& vEye, const CVector3& vLookAt)
{
	m_vEye = vEye;
	m_vLookAt = vLookAt;

	m_vForward = m_vLookAt-m_vEye;
	m_vForward.Normalize();

	m_vLookAt = m_vEye+m_vForward;

	m_vRight = m_vUp^m_vForward;
	m_vRight.Normalize();

	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vLookAt, m_vUp);

	SyncRotFromForward(m_vForward);
}

void CCamera::InitFromBBox(const CVector3& vMinBBox, const CVector3& vMaxBBox)
{
	m_vLookAt = (vMinBBox + vMaxBBox)/2.0f;
	m_vEye = m_vLookAt;
	m_vEye.z -= (vMaxBBox.z - vMinBBox.z)*3.0f;

	m_vForward = m_vLookAt-m_vEye;
	m_vForward.Normalize();

	m_vRight = m_vUp^m_vForward;
	m_vRight.Normalize();

	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vLookAt, m_vUp);
}

const CMatrix4x4& CCamera::GetViewMatrix() const
{
	return m_matView;
}

void CCamera::Rotate(float fRotY, float fRotX)
{
	m_fRotY += fRotY;
	m_fRotX += fRotX;

	if (m_fRotY > COEMath::PIX2) m_fRotY -= COEMath::PIX2;
	else if (m_fRotY < -COEMath::PIX2) m_fRotY += COEMath::PIX2;

	if (m_fRotX > COEMath::PIX2) m_fRotX -= COEMath::PIX2;
	else if (m_fRotX < -COEMath::PIX2) m_fRotX += COEMath::PIX2;

	m_vUp = CVector3(0.0f, 1.0f, 0.0f);
	m_vRight = CVector3(1.0f, 0.0f, 0.0f);
	m_vForward = CVector3(0.0f, 0.0f, 1.0f);

	CMatrix4x4 matRot;
	COEMath::BuildRotationYMatrix(matRot, m_fRotY);
	m_vRight = m_vRight * matRot;
	m_vForward = m_vForward * matRot;

	COEMath::BuildRotationMatrix(matRot, m_vRight, m_fRotX);
	m_vUp = m_vUp * matRot;
	m_vForward = m_vForward * matRot;

	m_vForward.Normalize();

	m_vRight = m_vUp ^ m_vForward;
	m_vRight.Normalize();

	m_vUp = m_vForward ^ m_vRight;
	m_vUp.Normalize();

	m_vLookAt = m_vEye + m_vForward;
	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vLookAt, m_vUp);
}

void CCamera::Move(const CVector3& vDir, float fDistance)
{
	m_vEye += vDir*fDistance;
	m_vLookAt += vDir*fDistance;

	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vLookAt, m_vUp);
}

const CVector3& CCamera::GetEyePos() const
{
	return m_vEye;
}

const CVector3& CCamera::GetLookAtPos() const
{
	return m_vLookAt;
}

const CVector3& CCamera::GetVectorUp() const
{
	return m_vUp;
}

const CVector3& CCamera::GetVectorForword() const
{
	return m_vForward;
}

const CVector3& CCamera::GetVectorRight() const
{
	return m_vRight;
}

void CCamera::SyncRotFromForward(const CVector3& vForward)
{
	CVector3 vDir = vForward;
	vDir.y = 0.0f;
	vDir.Normalize();
	m_fRotY = -acosf(vDir.z);

	vDir = m_vForward;
	vDir.x = 0.0f;
	vDir.Normalize();
	m_fRotX = -acosf(m_vForward.z);
}
