/*!
 * \file OEUIPicture.cpp
 * \date 10-27-2010 22:55:17
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEUI/OEUIPicture.h>
#include <OEUI/IOEUIRendererMgr.h>

COEUIPicture::COEUIPicture(COEUIWindow* pParent)
:COEUIWindow(pParent)
{
	m_pImage = g_pOEUIRendererMgr->CreateImageRenderer();
}

COEUIPicture::~COEUIPicture()
{
	SAFE_RELEASE(m_pImage);
}

void COEUIPicture::Render(float fDetailTime)
{
	m_pImage->Render(fDetailTime);
}

bool COEUIPicture::SetPicture(IOETexture* pTexture)
{
	m_pImage->SetTexture(pTexture);
	return true;
}

IOETexture* COEUIPicture::GetPicture()
{
	return m_pImage->GetTexture();
}

void COEUIPicture::SetPosition(const CPoint& pos)
{
	m_pImage->SetPosition(pos);
}

const CPoint& COEUIPicture::GetPosition()
{
	return m_pImage->GetPosition();
}

void COEUIPicture::SetSize(const CSize& size)
{
	return m_pImage->SetSize(size);
}

const CSize& COEUIPicture::GetSize()
{
	return m_pImage->GetSize();
}
