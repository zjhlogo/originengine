/*!
 * \file HatchingApp.h
 * \date 11-2-2010 20:11:14
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __HATCHINGAPP_H__
#define __HATCHINGAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>
#include <libOEMsg/OEMsgShaderParam.h>

class CHatchingApp : public CBaseApp
{
public:
	CHatchingApp();
	virtual ~CHatchingApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnSetupShaderParam(COEMsgShaderParam& msg);

private:
	IOEModel* m_pModel;
	CVector3 m_vLightPos;
	float m_bTimeStop;

};
#endif // __HATCHINGAPP_H__
