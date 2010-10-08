/*!
 * \file StringWindow.h
 * \date 10-8-2010 9:50:54
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __STRINGWINDOW_H__
#define __STRINGWINDOW_H__

#include <libOEUI/OEUIWindow.h>
#include <OEUI/IOEUIFont.h>
#include <OEUI/IOEUIString.h>

class CStringWindow : public COEUIWindow
{
public:
	CStringWindow(COEUIWindow* pParent);
	virtual ~CStringWindow();

	virtual void UpdateSelf(float fDetailTime);
	virtual void RenderSelf(float fDetailTime);

private:
	bool Init();
	void Destroy();

private:
	IOEUIFont* m_pFont;
	IOEUIString* m_pString;

};
#endif // __STRINGWINDOW_H__
