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
#include <OEUI/IOEUIResMgr.h>
#include <OEUI/IOEUIRendererMgr.h>

class CFPSWindow : public COEUIWindow
{
public:
	RTTI_DEF(CFPSWindow, COEUIWindow);

	CFPSWindow(COEUIWindow* pParent);
	virtual ~CFPSWindow();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	void ShowFPS(bool bShow);
	void ShowCameraPosRot(bool bShow);

private:
	void CalculateFPS();
	void CalculateCameraPos();

private:
	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;
	IOEUIFont* m_pFont;
	IOEUIStringRenderer* m_pStringFPS;
	IOEUIStringRenderer* m_pStringCameraPos;

	bool m_bShowFPS;
	bool m_bShowCameraPosRot;

};
#endif // __FPSWINDOW_H__
