/*!
 * \file OEUIImageRenderer_Impl.h
 * \date 10-26-2010 0:12:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEUIIMAGERENDERER_IMPL_H__
#define __OEUIIMAGERENDERER_IMPL_H__

#include <OEUI/IOEUIImageRenderer.h>

class COEUIImageRenderer_Impl : public IOEUIImageRenderer
{
public:
	RTTI_DEF(COEUIImageRenderer_Impl, IOEUIImageRenderer);

	COEUIImageRenderer_Impl();
	virtual ~COEUIImageRenderer_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual void SetTexture(IOETexture* pTexture, bool bAdjustSizeFromTexture = false);
	virtual IOETexture* GetTexture();

	virtual void SetPosition(const CPoint& pos);
	virtual const CPoint& GetPosition();

	virtual void SetSize(const CSize& size);
	virtual const CSize& GetSize();

private:
	bool Init();
	void Destroy();

private:
	IOETexture* m_pTexture;
	CPoint m_vPos;
	CSize m_Size;

};
#endif // __OEUIIMAGERENDERER_IMPL_H__
