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
	// TODO: 
}

void COEUIWindow::Render(float fDetailTime)
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

bool COEUIWindow::SetChildWindowLayer(COEUIWindow* pChild, WINDOW_LAYER eLayer, COEUIWindow* pChildRelative /*= NULL*/)
{
	switch (eLayer)
	{
	case WL_FRONT:
		{
			TV_WINDOW::iterator itfound = FindChild(pChild);
			if (itfound != m_vChildren.end())
			{
				m_vChildren.erase(itfound);
				m_vChildren.push_back(pChild);
				return true;
			}
		}
		break;
	case WL_BACK:
		{
			TV_WINDOW::iterator itfound = FindChild(pChild);
			if (itfound != m_vChildren.end())
			{
				m_vChildren.erase(itfound);
				m_vChildren.insert(m_vChildren.begin(), pChild);
				return true;
			}
		}
		break;
	case WL_NEXT:
		{
			TV_WINDOW::iterator itChild = FindChild(pChild);
			if (itChild == m_vChildren.end()) return false;

			TV_WINDOW::iterator itChildRelative = FindChild(pChildRelative);
			if (itChildRelative != m_vChildren.end())
			{
				m_vChildren.insert(itChildRelative+1, pChild);
				itChild = FindChild(pChild);
				m_vChildren.erase(itChild);
				return true;
			}
		}
		break;
	case WL_PREVIOUS:
		{
			TV_WINDOW::iterator itChild = FindChild(pChild);
			if (itChild == m_vChildren.end()) return false;

			TV_WINDOW::iterator itChildRelative = FindChild(pChildRelative);
			if (itChildRelative != m_vChildren.end())
			{
				m_vChildren.insert(itChildRelative, pChild);
				itChild = FindChild(pChild);
				m_vChildren.erase(itChild);
				return true;
			}
		}
		break;
	default:
		{
			return false;
		}
		break;
	}

	return true;
}

bool COEUIWindow::AddChild(COEUIWindow* pChild)
{
	// TODO: check if child exist
	m_vChildren.push_back(pChild);
	return true;
}

COEUIWindow::TV_WINDOW::iterator COEUIWindow::FindChild(COEUIWindow* pChild)
{
	for (TV_WINDOW::iterator it = m_vChildren.begin(); it != m_vChildren.end(); ++it)
	{
		if ((*it) == pChild) return it;
	}

	return m_vChildren.end();
}
