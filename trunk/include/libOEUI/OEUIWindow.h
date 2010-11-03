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

	enum WINDOW_LAYER
	{
		WL_UNKNOWN = 0,
		WL_FRONT,
		WL_BACK,
		WL_NEXT,
		WL_PREVIOUS,
	};

public:
	COEUIWindow(COEUIWindow* pParent);
	virtual ~COEUIWindow();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual void UpdateChildren(float fDetailTime);
	virtual void RenderChildren(float fDetailTime);

	virtual bool SetChildWindowLayer(COEUIWindow* pChild, WINDOW_LAYER eLayer, COEUIWindow* pChildRelative = NULL);

private:
	bool Init();
	void Destroy();

	bool AddChild(COEUIWindow* pChild);
	TV_WINDOW::iterator FindChild(COEUIWindow* pChild);

private:
	COEUIWindow* m_pParent;
	TV_WINDOW m_vChildren;

};
#endif // __OEUIWINDOW_H__
