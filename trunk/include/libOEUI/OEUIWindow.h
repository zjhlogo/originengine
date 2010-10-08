/*!
 * \file OEUIWindow.h
 * \date 10-8-2010 8:59:16
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEUIWINDOW_H__
#define __OEUIWINDOW_H__

#include "../OEUI/IOEUIRenderableObject.h"
#include <vector>

class COEUIWindow : public IOEUIRenderableObject
{
public:
	typedef std::vector<COEUIWindow*> TV_WINDOW;

public:
	COEUIWindow(COEUIWindow* pParent);
	virtual ~COEUIWindow();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual void UpdateSelf(float fDetailTime);
	virtual void RenderSelf(float fDetailTime);

private:
	bool Init();
	void Destroy();

	void UpdateChildren(float fDetailTime);
	void RenderChildren(float fDetailTime);

	bool AddChild(COEUIWindow* pChild);

private:
	COEUIWindow* m_pParent;
	TV_WINDOW m_vChildren;

};
#endif // __OEUIWINDOW_H__
