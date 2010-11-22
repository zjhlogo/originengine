/*!
 * \file CartoonApp.h
 * \date 11-2-2010 22:02:10
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __CARTOONAPP_H__
#define __CARTOONAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>
#include <libOEMsg/OEMsgShaderParam.h>

class CCartoonApp : public CBaseApp
{
public:
	CCartoonApp();
	virtual ~CCartoonApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnSetupShaderParam(COEMsgShaderParam& msg);

private:
	IOEModel* m_pModel;

};
#endif // __CARTOONAPP_H__
