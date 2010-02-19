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
	m_bOK = Init();
}

COEUIStringMgr_Impl::~COEUIStringMgr_Impl()
{
	Destroy();
	g_pOEUIStringMgr = NULL;
}

bool COEUIStringMgr_Impl::Init()
{
	// TODO: 
	return true;
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
		LOGOUT(TS("IOEUIStringMgr::CreateUIString Failed"));
		SAFE_DELETE(pString);
		return NULL;
	}

	return pString;
}
