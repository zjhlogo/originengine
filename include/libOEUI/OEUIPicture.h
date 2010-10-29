/*!
 * \file OEUIPicture.h
 * \date 10-27-2010 22:49:25
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEUIPICTURE_H__
#define __OEUIPICTURE_H__

#include "OEUIWindow.h"
#include "../OEUI/IOEUIImageRenderer.h"

class COEUIPicture : public COEUIWindow
{
public:
	RTTI_DEF(COEUIPicture, COEUIWindow);

	COEUIPicture(COEUIWindow* pParent);
	virtual ~COEUIPicture();

	virtual void Render(float fDetailTime);

	bool SetPicture(IOETexture* pTexture);
	IOETexture* GetPicture();

	void SetSize(const CSize& size);
	const CSize& GetSize();

private:
	IOEUIImageRenderer* m_pImage;

};
#endif // __OEUIPICTURE_H__
