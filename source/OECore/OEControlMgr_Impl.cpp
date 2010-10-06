/*!
 * \file OEControlMgr_Impl.cpp
 * \date 13-2-2010 13:16:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEControlMgr_Impl.h"
#include "OEControl/OESkinMeshControl_Impl.h"

#include <assert.h>

COEControlMgr_Impl::COEControlMgr_Impl()
{
	assert(!g_pOEControlMgr);
	g_pOEControlMgr = this;
	m_bOK = true;
}

COEControlMgr_Impl::~COEControlMgr_Impl()
{
	g_pOEControlMgr = NULL;
}

bool COEControlMgr_Impl::Initialize()
{
	AddControlHandle(new COESkinMeshControl_Impl());

	return true;
}

void COEControlMgr_Impl::Terminate()
{
	for (TM_CONTROL::iterator it = m_ControlMap.begin(); it != m_ControlMap.end(); ++it)
	{
		IOEControl* pControl = it->second;
		SAFE_RELEASE(pControl);
	}
	m_ControlMap.clear();
}

bool COEControlMgr_Impl::AddControlHandle(IOEControl* pControl)
{
	if (GetControl(pControl->GetRtti()->GetClassName())) return false;

	m_ControlMap.insert(std::make_pair(pControl->GetRtti()->GetClassName(), pControl));
	return true;
}

IOEControl* COEControlMgr_Impl::GetControl(const tstring& strClassName)
{
	TM_CONTROL::iterator itfound = m_ControlMap.find(strClassName);
	if (itfound == m_ControlMap.end()) return NULL;

	return itfound->second;
}
