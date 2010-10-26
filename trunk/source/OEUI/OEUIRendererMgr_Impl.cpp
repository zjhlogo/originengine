/*!
 * \file OEUIRendererMgr_Impl.cpp
 * \date 10-26-2010 0:07:16
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEUIRendererMgr_Impl.h"
#include "OEUIStringRenderer_Impl.h"
#include "OEUIImageRenderer_Impl.h"
#include <OEBase/IOELogFileMgr.h>
#include <assert.h>

COEUIRendererMgr_Impl::COEUIRendererMgr_Impl()
{
	assert(!g_pOEUIRendererMgr);
	g_pOEUIRendererMgr = this;
	m_bOK = Init();
}

COEUIRendererMgr_Impl::~COEUIRendererMgr_Impl()
{
	Destroy();
	g_pOEUIRendererMgr = NULL;
}

bool COEUIRendererMgr_Impl::Init()
{
	// TODO: 
	return true;
}

void COEUIRendererMgr_Impl::Destroy()
{
	// TODO: 
}

bool COEUIRendererMgr_Impl::Initialize()
{
	// TODO: 
	return true;
}

void COEUIRendererMgr_Impl::Terminate()
{
	// TODO: 
}

IOEUIStringRenderer* COEUIRendererMgr_Impl::CreateStringRenderer(IOEUIFont* pFont)
{
	COEUIStringRenderer_Impl* pString = new COEUIStringRenderer_Impl(pFont);
	if (!pString || !pString->IsOK())
	{
		LOGOUT(TS("IUIRendererMgr::CreateStringRenderer Failed"));
		SAFE_DELETE(pString);
		return NULL;
	}

	return pString;
}

IOEUIImageRenderer* COEUIRendererMgr_Impl::CreateImageRenderer()
{
	COEUIImageRenderer_Impl* pImage = new COEUIImageRenderer_Impl();
	if (!pImage || !pImage->IsOK())
	{
		LOGOUT(TS("IUIRendererMgr::CreateImageRenderer Failed"));
		SAFE_DELETE(pImage);
		return NULL;
	}

	return pImage;
}
