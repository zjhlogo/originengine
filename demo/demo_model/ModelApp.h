/*!
 * \file ModelApp.h
 * \date 1-3-2010 21:43:12
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELAPP_H__
#define __MODELAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>
#include <vector>

class CModelApp : public CBaseApp
{
public:
	CModelApp();
	virtual ~CModelApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnSetupShaderParam(COEMsgShaderParam& msg);

private:
	IOEModel* m_pModel;

};
#endif // __MODELAPP_H__
