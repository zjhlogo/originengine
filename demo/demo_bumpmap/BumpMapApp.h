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
#include <OECore/IOEModel.h>

class CBumpMapApp : public CBaseApp
{
public:
	CBumpMapApp();
	virtual ~CBumpMapApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	virtual bool OnKeyDown(COEMsgKeyboard& msg);
	bool OnSetupShaderParam(COEMsgShaderParam& msg);

private:
	CSimpleShape* m_pSimpleShape;
	CVector3 m_vLightPos;
	IOEModel* m_pModel;

};

#endif // __BUMPMAPAPP_H__
