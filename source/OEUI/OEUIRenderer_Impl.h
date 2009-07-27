/*!
 * \file OEUIRenderer_Impl.h
 * \date 27-7-2009 17:58:05
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIRENDERER_IMPL_H__
#define __OEUIRENDERER_IMPL_H__

#include <OEUI/IOEUIRenderer.h>

class COEUIRenderer_Impl : public IOEUIRenderer
{
public:
	COEUIRenderer_Impl();
	virtual ~COEUIRenderer_Impl();

	virtual void SetTexture(IOETexture* pTexture);
	virtual IOETexture* GetTexture() const;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void FlushAll();

	bool IsOK();

private:
	void Init();
	void Destroy();

	bool Create();

private:
	IOEVertDecl* m_pDecl;
	IOETexture* m_pTexture;
	bool m_bOK;

};
#endif // __OEUIRENDERER_IMPL_H__
