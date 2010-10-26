/*!
 * \file UIApp.h
 * \date 27-7-2009 15:37:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __UIAPP_H__
#define __UIAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>
#include <libOEMsg/OEMsgCommand.h>

class CUIApp : public CBaseApp
{
public:
	CUIApp();
	virtual ~CUIApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();
	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnPreRender(COEMsgCommand& msg);
	bool OnPostRender(COEMsgCommand& msg);

private:
	IOEModel* m_pModel;
	IOETexture* m_pRenderTargetTexture;

};

#endif // __UIAPP_H__
