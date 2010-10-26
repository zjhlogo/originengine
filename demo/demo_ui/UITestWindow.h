/*!
 * \file UITestWindow.h
 * \date 10-26-2010 0:48:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __UITESTWINDOW_H__
#define __UITESTWINDOW_H__

#include <libOEUI/OEUIWindow.h>
#include <OEUI/IOEUIFont.h>
#include <OEUI/IOEUIStringRenderer.h>
#include <OEUI/IOEUIImageRenderer.h>

class CUITestWindow : public COEUIWindow
{
public:
	CUITestWindow(COEUIWindow* pParent);
	virtual ~CUITestWindow();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	bool Init();
	void Destroy();

private:
	IOEUIFont* m_pFont;
	IOEUIStringRenderer* m_pString;
	IOETexture* m_pTexture;
	IOEUIImageRenderer* m_pImage;

};
#endif // __UITESTWINDOW_H__
