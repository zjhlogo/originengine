/*!
 * \file ConverterMgr.cpp
 * \date 1-3-2010 21:23:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ConverterMgr.h"
#include "Ms3dConverter_Impl.h"

CConverterMgr::CConverterMgr()
{
	Init();
}

CConverterMgr::~CConverterMgr()
{
	Destroy();
}

void CConverterMgr::Init()
{
	m_vConverter.push_back(new CMs3dConverter_Impl());
}

void CConverterMgr::Destroy()
{
	for (TV_CONVERTER::iterator it = m_vConverter.begin(); it != m_vConverter.end(); ++it)
	{
		IConverter* pBaseConv = (*it);
		SAFE_DELETE(pBaseConv);
	}
	m_vConverter.clear();
}

CConverterMgr& CConverterMgr::Get()
{
	static CConverterMgr s_ConverterMgr;
	return s_ConverterMgr;
}

bool CConverterMgr::DoConvert(const tstring& strFileIn, const tstring& strFileOut)
{
	for (TV_CONVERTER::iterator it = m_vConverter.begin(); it != m_vConverter.end(); ++it)
	{
		IConverter* pConv = (*it);
		if (pConv->CanConvert(strFileIn))
		{
			return pConv->DoConvert(strFileIn, strFileOut);
		}
	}

	return false;
}
