/*!
 * \file OEModelMgr_Impl.cpp
 * \date 9-8-2009 23:43:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEModelMgr_Impl.h"
#include "OEModel_Impl.h"

#include <IOELogFileMgr.h>

COEModelMgr_Impl::COEModelMgr_Impl()
{
	g_pOEModelMgr = this;
	Init();
	m_bOK = true;
}

COEModelMgr_Impl::~COEModelMgr_Impl()
{
	Destroy();
	g_pOEModelMgr = NULL;
}

void COEModelMgr_Impl::Init()
{
	// TODO: 
}

void COEModelMgr_Impl::Destroy()
{
	// TODO: 
}

IOEModel* COEModelMgr_Impl::CreateModelFromFile(const tstring& strFileName)
{
	COEModel_Impl* pModel = new COEModel_Impl(strFileName);
	if (!pModel || !pModel->IsOK())
	{
		LOGOUT(TS("IOEModelMgr::CreateModelFromFile Failed"));
		SAFE_DELETE(pModel);
		return NULL;
	}

	return pModel;
}
