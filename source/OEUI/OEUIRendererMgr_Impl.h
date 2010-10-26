/*!
 * \file OEUIRendererMgr_Impl.h
 * \date 10-26-2010 0:07:01
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEUIRENDERERMGR_IMPL_H__
#define __OEUIRENDERERMGR_IMPL_H__

#include <OEUI/IOEUIRendererMgr.h>

class COEUIRendererMgr_Impl : public IOEUIRendererMgr
{
public:
	COEUIRendererMgr_Impl();
	virtual ~COEUIRendererMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual IOEUIStringRenderer* CreateStringRenderer(IOEUIFont* pFont);
	virtual IOEUIImageRenderer* CreateImageRenderer();

private:
	bool Init();
	void Destroy();

private:

};
#endif // __OEUIRENDERERMGR_IMPL_H__
