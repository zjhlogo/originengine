/*!
 * \file OEUIStringMgr_Impl.cpp
 * \date 28-7-2009 9:40:57
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIStringMgr_Impl.h"
#include "OEUIString_Impl.h"

#include <IOELogFileMgr.h>

COEUIStringMgr_Impl::COEUIStringMgr_Impl()
{
	g_pOEUIStringMgr = this;
	Init();
	m_bOK = true;
}

COEUIStringMgr_Impl::~COEUIStringMgr_Impl()
{
	Destroy();
	g_pOEUIStringMgr = NULL;
}

void COEUIStringMgr_Impl::Init()
{
	// TODO: 
}

void COEUIStringMgr_Impl::Destroy()
{
	// TODO: 
}

IOEUIString* COEUIStringMgr_Impl::CreateUIString(IOEUIFont* pFont)
{
	COEUIString_Impl* pString = new COEUIString_Impl(pFont);
	if (!pString || !pString->IsOK())
	{
		LOGOUT(t("IOEUIStringMgr::CreateUIString Failed"));
		SAFE_DELETE(pString);
		return NULL;
	}

	return pString;
}
