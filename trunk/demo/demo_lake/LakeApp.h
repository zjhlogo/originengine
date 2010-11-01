/*!
 * \file LakeApp.h
 * \date 10-27-2010 9:28:45
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __LAKEAPP_H__
#define __LAKEAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEModel.h>

class CLakeApp : public CBaseApp
{
public:
	CLakeApp();
	virtual ~CLakeApp();

	virtual bool UserDataInit();
	virtual void UserDataTerm();

	virtual void Update(float fDetailTime);

private:
	void Init();
	void Destroy();

private:
	IOEModel* m_pModelSky;
	IOEModel* m_pModelLand;
	IOEModel* m_pModelLake;
	IOETexture* m_pRenderTargetReflect;
	IOETexture* m_pRenderTargetRefract;

};
#endif // __LAKEAPP_H__
