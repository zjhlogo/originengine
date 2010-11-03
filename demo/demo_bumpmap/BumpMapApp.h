/*!
 * \file BumpMapApp.h
 * \date 6-7-2009 9:23:04
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __BUMPMAPAPP_H__
#define __BUMPMAPAPP_H__

#include "../common/BaseApp.h"
#include "../common/SimpleShape.h"

#include <libOEMsg/OEMsgShaderParam.h>
#include <OECore/IOEModel.h>

class CBumpMapApp : public CBaseApp
{
public:
	CBumpMapApp();
	virtual ~CBumpMapApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnKeyDown(COEMsgKeyboard& msg);
	bool OnSetupShaderParam(COEMsgShaderParam& msg);

private:
	CSimpleShape* m_pSimpleShape;
	CVector3 m_vLightPos;
	IOEModel* m_pModel;
	bool m_bTimeStop;

};

#endif // __BUMPMAPAPP_H__
