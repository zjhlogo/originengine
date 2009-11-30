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
	m_vEye = CVector3(0.0f, 3.0f, -5.0f);
	m_vLookAt = CVector3(0.0f, 0.0f, 0.0f);
	m_vUp = CVector3(0.0f, 1.0f, 0.0f);

	m_vForword = m_vLookAt-m_vEye;
	m_vForword.Normalize();

	m_vLookAt = m_vEye+m_vForword;

	m_vRight = m_vUp^m_vForword;
	m_vRight.Normalize();

	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vLookAt, m_vUp);
}

void CCamera::Destroy()
{
	// TODO: 
}

void CCamera::Initialize(const CVector3& vEye, const CVector3& vLookAt)
{
	m_vEye = vEye;
	m_vLookAt = vLookAt;

	m_vForword = m_vLookAt-m_vEye;
	m_vForword.Normalize();

	m_vLookAt = m_vEye+m_vForword;

	m_vRight = m_vUp^m_vForword;
	m_vRight.Normalize();

	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vLookAt, m_vUp);
}

const CMatrix4x4& CCamera::GetViewMatrix() const
{
	return m_matView;
}

void CCamera::Rotate(const CVector3& vDir, float fRadian)
{
	CQuaternion qr(vDir, fRadian);
	qr.Normalize();

	CQuaternion t = qr*m_vForword*(-qr);
	CVector3 vNewForword(t.x, t.y, t.z);
	vNewForword.Normalize();

	float fCosTheta = fabsf(vNewForword*m_vUp);
	if (fCosTheta > 0.99f) return;

	m_vLookAt = m_vEye+vNewForword;

	m_vForword = vNewForword;
	m_vRight = m_vUp^m_vForword;
	m_vRight.Normalize();

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
	return m_vForword;
}

const CVector3& CCamera::GetVectorRight() const
{
	return m_vRight;
}
