/*!
 * \file BaseApp.h
 * \date 8-18-2010 10:50:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BASEAPP_H__
#define __BASEAPP_H__

#include <libOEBase/IOEApp.h>
#include <OECore/IOEModel.h>
#include "Camera.h"
#include "FPSWindow.h"

class CBaseApp : public IOEApp
{
public:
	CBaseApp();
	virtual ~CBaseApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool UserDataInit() = 0;
	virtual void UserDataTerm() = 0;

	virtual void PreUpdate(float fDetailTime);
	virtual void PostUpdate(float fDetailTime);

	void ResetCameraPosRot(IOEModel* pModel);
	void ResetCameraPosRot(const CVector3& vPos, float fRotY, float fRotX);
	void ResetCameraPosRot(const CVector3& vPos, const CQuaternion& qRot);

	void SetMovementSpeed(float fSpeed);
	float GetMovementSpeed();

	void SetRotationSpeed(float fSpeed);
	float GetRotationSpeed();

	void ShowFPS(bool bShow);
	void ShowCameraPosRot(bool bShow);

private:
	void Init();
	void Destroy();

private:
	CCamera* m_pCamera;
	CFPSWindow* m_pFPS;

};

#endif // __BASEAPP_H__
