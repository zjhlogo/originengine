/*!
 * \file MeshApp.h
 * \date 31-7-2009 20:41:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MESHAPP_H__
#define __MESHAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>
#include <libOEMsg/OEMsgShaderParam.h>

class CMeshApp : public CBaseApp
{
public:
	CMeshApp();
	virtual ~CMeshApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnSetupShaderParam(COEMsgShaderParam& msg);

private:
	IOEModel* m_pModel;
	CVector3 m_vOffset;

};

#endif // __MESHAPP_H__
