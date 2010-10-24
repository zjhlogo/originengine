/*!
 * \file Camera.h
 * \date 28-5-2009 21:13:35
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <OECore/IOECamera.h>
#include <libOEMsg/OEMsgMouse.h>
#include <libOEMsg/OEMsgKeyboard.h>

class CCamera : public IOECamera
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

public:
	RTTI_DEF(CCamera, IOECamera);

	CCamera();
	virtual ~CCamera();

	virtual const CMatrix4x4& GetViewMatrix() const;

	virtual void SetPosition(const CVector3& pos);
	virtual const CVector3& GetPosition() const;

	virtual const CVector3& GetVectorUp() const;
	virtual const CVector3& GetVectorForword() const;
	virtual const CVector3& GetVectorRight() const;

	virtual void SetTargetNode(IOENode* pNode);
	virtual IOENode* GetTargetNode();

	virtual void Update(float fDetailTime);

	void InitFromBBox(const CVector3& vMinBBox, const CVector3& vMaxBBox);

	void Rotate(float fRotYDetail, float fRotXDetail);
	void RotateTo(float fRotY, float fRotX);
	void Move(const CVector3& vDir, float fDistance);
	void MoveTo(const CVector3& vPos);

private:
	void Init();
	void Destroy();

	void SyncRotFromForward(const CVector3& vForward);

	bool OnLButtonDown(COEMsgMouse& msg);
	bool OnLButtonUp(COEMsgMouse& msg);
	bool OnMouseMove(COEMsgMouse& msg);
	bool OnKeyUp(COEMsgKeyboard& msg);
	bool OnKeyDown(COEMsgKeyboard& msg);

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

private:
	CVector3 m_vEye;

	CVector3 m_vUp;
	CVector3 m_vForward;
	CVector3 m_vRight;

	float m_fRotX;
	float m_fRotY;

	CMatrix4x4 m_matView;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];
	bool m_bFirstTimeUpdate;

	IOENode* m_pTargetNode;

};
#endif // __CAMERA_H__
