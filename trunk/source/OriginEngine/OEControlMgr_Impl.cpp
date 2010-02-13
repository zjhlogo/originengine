/*!
 * \file OEControlMgr_Impl.cpp
 * \date 13-2-2010 13:16:33
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OEControlMgr_Impl.h"
#include "OESkinMeshControl_Impl.h"

COEControlMgr_Impl::COEControlMgr_Impl()
{
	g_pOEControlMgr = this;
	Init();
}

COEControlMgr_Impl::~COEControlMgr_Impl()
{
	Destroy();
	g_pOEControlMgr = NULL;
}

void COEControlMgr_Impl::Init()
{
	AddControlHandle(new COESkinMeshControl_Impl());
	m_bOK = true;
}

void COEControlMgr_Impl::Destroy()
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
	if (GetControl(pControl->GetType())) return false;

	m_ControlMap.insert(std::make_pair(pControl->GetType(), pControl));
	return true;
}

IOEControl* COEControlMgr_Impl::GetControl(uint nID)
{
	TM_CONTROL::iterator itfound = m_ControlMap.find(nID);
	if (itfound == m_ControlMap.end()) return NULL;

	return itfound->second;
}
