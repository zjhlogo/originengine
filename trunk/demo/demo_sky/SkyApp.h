/*!
 * \file SkyApp.h
 * \date 10-24-2010 10:08:02
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __SKYAPP_H__
#define __SKYAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>
#include <libOEMsg/OEMsgShaderParam.h>

class CSkyApp : public CBaseApp
{
public:
	CSkyApp();
	virtual ~CSkyApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

private:
	IOEModel* m_pModel;

};
#endif // __SKYAPP_H__
