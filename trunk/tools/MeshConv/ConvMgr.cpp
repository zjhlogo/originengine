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
	m_vConvList.push_back(new CMs3dConv_Impl());
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

bool CConvMgr::DoConvert(const tstring& strFileIn, const tstring& strFileOut)
{
	for (TV_BASE_CONV::iterator it = m_vConvList.begin(); it != m_vConvList.end(); ++it)
	{
		IBaseConv* pConv = (*it);
		if (pConv->CanConvert(strFileIn))
		{
			return pConv->DoConvert(strFileIn, strFileOut);
		}
	}

	return false;
}
