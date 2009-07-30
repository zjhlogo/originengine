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
#include "OEUIVertexCache.h"

class COEUIRenderer_Impl : public IOEUIRenderer
{
public:
	enum CONST_DEFINE
	{
		VERTEX_CACHE_SIZE = 4096*32,
		INDEX_CACHE_COUNT = (4096*3)/2+2,
		VERTEX_CACHE_COUNT = 1,
	};

public:
	COEUIRenderer_Impl();
	virtual ~COEUIRenderer_Impl();

	virtual void SetTexture(IOETexture* pTexture);
	virtual IOETexture* GetTexture() const;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void FlushAll();

private:
	void Init();
	void Destroy();

	bool Create();

private:
	IOEVertDecl* m_pDecl;
	IOETexture* m_pTexture;

	COEUIVertexCache* m_pVertsCache[VERTEX_CACHE_COUNT];
	bool m_bInitialized;

};
#endif // __OEUIRENDERER_IMPL_H__
