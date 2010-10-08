/*!
 * \file OEUIWindow.cpp
 * \date 10-8-2010 9:00:26
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEUI/OEUIWindow.h>

COEUIWindow::COEUIWindow(COEUIWindow* pParent)
{
	m_bOK = Init();
	m_pParent = pParent;
	if (m_pParent) m_pParent->AddChild(this);
}

COEUIWindow::~COEUIWindow()
{
	Destroy();
}

bool COEUIWindow::Init()
{
	m_pParent = NULL;
	return true;
}

void COEUIWindow::Destroy()
{
	// destroy all children
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		COEUIWindow* pChild = (*it);
		SAFE_RELEASE(pChild);
	}
	m_vChildren.clear();
}

void COEUIWindow::Update(float fDetailTime)
{
	UpdateSelf(fDetailTime);
	UpdateChildren(fDetailTime);
}

void COEUIWindow::Render(float fDetailTime)
{
	RenderSelf(fDetailTime);
	RenderChildren(fDetailTime);
}

void COEUIWindow::UpdateSelf(float fDetailTime)
{
	// TODO: 
}

void COEUIWindow::RenderSelf(float fDetailTime)
{
	// TODO: 
}

void COEUIWindow::UpdateChildren(float fDetailTime)
{
	// update children
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		COEUIWindow* pChild = (*it);
		pChild->Update(fDetailTime);
	}
}

void COEUIWindow::RenderChildren(float fDetailTime)
{
	// render children
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		COEUIWindow* pChild = (*it);
		pChild->Render(fDetailTime);
	}
}

bool COEUIWindow::AddChild(COEUIWindow* pChild)
{
	// TODO: check if child exist
	m_vChildren.push_back(pChild);
	return true;
}
