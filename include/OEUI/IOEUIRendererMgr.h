/*!
 * \file IOEUIRendererMgr.h
 * \date 10-25-2010 23:59:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEUIRENDERERMGR_H__
#define __IOEUIRENDERERMGR_H__

#include "../OECore/IOEMgr.h"
#include "IOEUIStringRenderer.h"
#include "IOEUIImageRenderer.h"

class IOEUIRendererMgr : public IOEMgr
{
public:
	RTTI_DEF(IOEUIRendererMgr, IOEMgr);

	IOEUIRendererMgr() {};
	virtual ~IOEUIRendererMgr() {};

	virtual IOEUIStringRenderer* CreateStringRenderer(IOEUIFont* pFont) = 0;
	virtual IOEUIImageRenderer* CreateImageRenderer() = 0;

};

extern IOEUIRendererMgr* g_pOEUIRendererMgr;
#endif // __IOEUIRENDERERMGR_H__
