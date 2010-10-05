/*!
 * \file OEUIFont_Impl.cpp
 * \date 27-7-2009 17:57:43
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEUIFont_Impl.h"

COEUIFont_Impl::COEUIFont_Impl()
:IOEUIFont(TS("COEUIFont_Impl"))
{
	m_bOK = Init();
}

COEUIFont_Impl::~COEUIFont_Impl()
{
	Destroy();
}

bool COEUIFont_Impl::Init()
{
	// TODO: 
	return true;
}

void COEUIFont_Impl::Destroy()
{
	// TODO: 
}

float COEUIFont_Impl::GetLineHeight() const
{
	// TODO: 
	return 0;
}

const IOEUIFont::CHAR_INFO* COEUIFont_Impl::GetCharInfo(int nID) const
{
	// TODO: 
	return NULL;
}

float COEUIFont_Impl::GetKerning(int nFirstID, int nSecondID) const
{
	// TODO: 
	return 0.0f;
}
