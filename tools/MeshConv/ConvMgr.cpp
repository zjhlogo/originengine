/*!
 * \file ConvMgr.cpp
 * \date 10-20-2009 15:24:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ConvMgr.h"
#include "Ms3dConv_Impl.h"

CConvMgr::CConvMgr()
{
	Init();
}

CConvMgr::~CConvMgr()
{
	Destroy();
}

void CConvMgr::Init()
{
	m_bInitialized = false;
}

void CConvMgr::Destroy()
{
	for (TV_BASE_CONV::iterator it = m_vConvList.begin(); it != m_vConvList.end(); ++it)
	{
		IBaseConv* pBaseConv = (*it);
		SAFE_DELETE(pBaseConv);
	}
	m_vConvList.clear();
}

CConvMgr& CConvMgr::Get()
{
	static CConvMgr s_ConvMgr;
	return s_ConvMgr;
}

bool CConvMgr::Initialized()
{
	if (m_bInitialized) return true;

	m_vConvList.push_back(new CMs3dConv_Impl());

	m_bInitialized = true;
	return true;
}

bool CConvMgr::CanConvert(const tstring& strFile)
{
	// TODO: 
	return true;
}

bool CConvMgr::DoConvert(const tstring& strFileIn, const tstring& strFileOut)
{
	// TODO: 
	IBaseConv* pBaseConv = NULL;
	for (TV_BASE_CONV::iterator it = m_vConvList.begin(); it != m_vConvList.end(); ++it)
	{
		// TODO: 
		pBaseConv = (*it);
		break;
	}

	return pBaseConv->DoConvert(strFileIn, strFileOut);
}
