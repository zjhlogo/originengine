/*!
 * \file OERenderMgr_Impl.h
 * \date 13-2-2010 13:04:45
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OERENDERMGR_IMPL_H__
#define __OERENDERMGR_IMPL_H__

#include <OECore/IOERenderMgr.h>
#include <map>

class COERenderMgr_Impl : public IOERenderMgr
{
public:
	typedef std::map<tstring, IOERender*> TM_RENDER;

public:
	RTTI_DEF(COERenderMgr_Impl, IOERenderMgr);

	COERenderMgr_Impl();
	virtual ~COERenderMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual bool AddRenderHandle(IOERender* pRender);
	virtual IOERender* GetRender(const tstring& strClassName);

private:
	TM_RENDER m_RenderMap;

};
#endif // __OERENDERMGR_IMPL_H__
