/*!
 * \file Camera.cpp
 * \date 28-5-2009 21:13:45
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "Camera.h"
#include <OECore/IOECore.h>
#include <OECore/IOEDevice.h>
#include <OECore/IOERenderSystem.h>
#include <libOEMsg/OEMsgList.h>

CCamera::CCamera()
{
	Init();

	// registe message
	g_pOEDevice->RegisterEvent(OMI_LBUTTON_DOWN, this, (MSG_FUNC)&CCamera::OnLButtonDown);
	g_pOEDevice->RegisterEvent(OMI_LBUTTON_UP, this, (MSG_FUNC)&CCamera::OnLButtonUp);
	g_pOEDevice->RegisterEvent(OMI_MOUSE_MOVE, this, (MSG_FUNC)&CCamera::OnMouseMove);
	g_pOEDevice->RegisterEvent(OMI_KEY_DOWN, this, (MSG_FUNC)&CCamera::OnKeyDown);
	g_pOEDevice->RegisterEvent(OMI_KEY_UP, this, (MSG_FUNC)&CCamera::OnKeyUp);
}

CCamera::~CCamera()
{
	Destroy();
}

void CCamera::Init()
{
	m_bLButtonDown = false;
	m_nMouseDetailX = 0;
	m_nMouseDetailY = 0;
	memset(m_KeyDown, 0, sizeof(m_KeyDown));
	m_bFirstTimeUpdate = true;

	m_pTargetNode = NULL;

	m_fRotX = 0.0f;
	m_fRotY = 0.0f;

	m_fMovementSpeed = 50.0f;
	m_fRotationSpeed = 0.3f;

	RotateTo(0.0f, 0.0f);
}

void CCamera::Destroy()
{
	// TODO: 
}

const CMatrix4x4& CCamera::GetViewMatrix() const
{
	return m_matView;
}

void CCamera::SetPosition(const CVector3& pos)
{
	m_vEye = pos;
	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vEye+m_vForward, m_vUp);

	if (m_pTargetNode) m_pTargetNode->SetPosition(m_vEye);
}

const CVector3& CCamera::GetPosition() const
{
	return m_vEye;
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

void CCamera::SetTargetNode(IOENode* pNode)
{
	m_pTargetNode = pNode;
	if (m_pTargetNode)
	{
		m_pTargetNode->SetPosition(m_vEye);

		CQuaternion qRot;
		COEMath::BuildQuaternionFromEulerYZX(qRot, m_fRotX, m_fRotY, 0.0f);
		m_pTargetNode->SetRotation(qRot);
	}
}

IOENode* CCamera::GetTargetNode()
{
	return m_pTargetNode;
}

void CCamera::Update(float fDetailTime)
{
	bool bRot = UpdateRotation(fDetailTime);
	bool bMov = UpdateMovement(fDetailTime);
	if (bRot || bMov || m_bFirstTimeUpdate) g_pOERenderSystem->SetTransform(TT_VIEW, m_matView);
	m_bFirstTimeUpdate = false;
}

void CCamera::InitFromBBox(const CVector3& vMinBBox, const CVector3& vMaxBBox)
{
	CVector3 vPos = (vMinBBox + vMaxBBox)/2.0f;
	vPos.z -= (vMaxBBox.z - vMinBBox.z)*3.0f;

	RotateTo(0.0f, 0.0f);
	MoveTo(vPos);
}

void CCamera::Rotate(float fRotYDetail, float fRotXDetail)
{
	RotateTo(m_fRotY+fRotYDetail, m_fRotX+fRotXDetail);
}

void CCamera::RotateTo(float fRotY, float fRotX)
{
	m_fRotY = fRotY;
	m_fRotX = fRotX;

	if (m_fRotY > COEMath::PIX2) m_fRotY -= COEMath::PIX2;
	else if (m_fRotY < -COEMath::PIX2) m_fRotY += COEMath::PIX2;

	if (m_fRotX > COEMath::PIX2) m_fRotX -= COEMath::PIX2;
	else if (m_fRotX < -COEMath::PIX2) m_fRotX += COEMath::PIX2;

	m_vRight.Init(1.0f, 0.0f, 0.0f);
	m_vUp.Init(0.0f, 1.0f, 0.0f);
	m_vForward.Init(0.0f, 0.0f, 1.0f);

	CMatrix4x4 matRot;
	COEMath::BuildRotationYMatrix(matRot, m_fRotY);
	m_vRight = m_vRight*matRot;
	m_vForward = m_vForward*matRot;

	COEMath::BuildRotationMatrix(matRot, m_vRight, m_fRotX);
	m_vUp = m_vUp*matRot;
	m_vForward = m_vForward*matRot;
	m_vForward.Normalize();

	m_vRight = m_vUp^m_vForward;
	m_vRight.Normalize();

	m_vUp = m_vForward^m_vRight;
	m_vUp.Normalize();

	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vEye+m_vForward, m_vUp);

	if (m_pTargetNode)
	{
		CQuaternion qRot;
		COEMath::BuildQuaternionFromEulerYZX(qRot, m_fRotX, m_fRotY, 0.0f);
		m_pTargetNode->SetRotation(qRot);
	}
}

void CCamera::Move(const CVector3& vDir, float fDistance)
{
	m_vEye += vDir*fDistance;
	MoveTo(m_vEye+vDir*fDistance);
}

void CCamera::MoveTo(const CVector3& vPos)
{
	m_vEye = vPos;
	COEMath::BuildLookAtMatrixLH(m_matView, m_vEye, m_vEye+m_vForward, m_vUp);

	if (m_pTargetNode) m_pTargetNode->SetPosition(m_vEye);
}

bool CCamera::OnLButtonDown(COEMsgMouse& msg)
{
	m_bLButtonDown = true;
	return true;
}

bool CCamera::OnLButtonUp(COEMsgMouse& msg)
{
	m_bLButtonDown = false;
	return true;
}

bool CCamera::OnMouseMove(COEMsgMouse& msg)
{
	if (!m_bLButtonDown) return true;
	m_nMouseDetailX = msg.GetPosX();
	m_nMouseDetailY = msg.GetPosY();
	return true;
}

bool CCamera::OnKeyUp(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();

	m_KeyDown[nKeyCode] = false;
	return true;
}

bool CCamera::OnKeyDown(COEMsgKeyboard& msg)
{
	uint nKeyCode = msg.GetKeyCode();

	m_KeyDown[nKeyCode] = true;
	if (m_KeyDown[0x1B]) g_pOECore->End();		// TODO: 0x1B == VK_ESCAPE

	return true;
}

bool CCamera::UpdateMovement(float fDetailTime)
{
	bool bUpdateMovement = m_KeyDown['W'] || m_KeyDown['S'] || m_KeyDown['A'] || m_KeyDown['D'];
	if (!bUpdateMovement) return false;

	if (m_KeyDown['W']) Move(GetVectorForword(), m_fMovementSpeed*fDetailTime);
	if (m_KeyDown['S']) Move(GetVectorForword(), -m_fMovementSpeed*fDetailTime);
	if (m_KeyDown['D']) Move(GetVectorRight(), m_fMovementSpeed*fDetailTime);
	if (m_KeyDown['A']) Move(GetVectorRight(), -m_fMovementSpeed*fDetailTime);

	return true;
}

bool CCamera::UpdateRotation(float fDetailTime)
{
	if (!m_bLButtonDown) return false;
	if (m_nMouseDetailX == 0 && m_nMouseDetailY == 0) return false;

	if (m_nMouseDetailX || m_nMouseDetailY)
	{
		Rotate(-m_nMouseDetailX*fDetailTime*m_fRotationSpeed, -m_nMouseDetailY*fDetailTime*m_fRotationSpeed);
	}

	m_nMouseDetailY = 0;
	m_nMouseDetailX = 0;

	return true;
}
