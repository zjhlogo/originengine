/*!
 * \file UIApp.h
 * \date 27-7-2009 15:37:24
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __UIAPP_H__
#define __UIAPP_H__

#include "../common/BaseApp.h"

#include <OEUI/IOEUIFont.h>
#include <OEUI/IOEUIString.h>

class CUIApp : public CBaseApp
{
public:
	CUIApp();
	virtual ~CUIApp();

	virtual bool Initialize();
	virtual void Terminate();

	//virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

private:
	IOEUIFont* m_pFont;
	IOEUIString* m_pString;
	uint m_nPerformCount;

};

#endif // __UIAPP_H__
