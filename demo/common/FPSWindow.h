/*!
 * \file FPSWindow.h
 * \date 10-8-2010 9:40:06
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __FPSWINDOW_H__
#define __FPSWINDOW_H__

#include <libOEUI/OEUIWindow.h>
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

class CFPSWindow : public COEUIWindow
{
public:
	RTTI_DEF(CFPSWindow, COEUIWindow);

	CFPSWindow(COEUIWindow* pParent);
	virtual ~CFPSWindow();

	virtual void UpdateSelf(float fDetailTime);
	virtual void RenderSelf(float fDetailTime);

private:
	void CalculateFPS();

private:
	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;
	IOEUIFont* m_pFontFPS;
	IOEUIString* m_pStringFPS;

};
#endif // __FPSWINDOW_H__
