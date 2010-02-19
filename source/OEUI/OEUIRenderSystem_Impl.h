/*!
 * \file OEUIRenderSystem_Impl.h
 * \date 19-2-2010 19:52:06
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIRENDERSYSTEM_IMPL_H__
#define __OEUIRENDERSYSTEM_IMPL_H__

#include <OEUI/IOEUIRenderSystem.h>
#include "OEUIVertexCache.h"

class COEUIRenderSystem_Impl : public IOEUIRenderSystem
{
public:
	enum CONST_DEFINE
	{
		VERTEX_CACHE_SIZE = 4096*32,
		INDEX_CACHE_COUNT = (4096*3)/2+2,
		VERTEX_CACHE_COUNT = 1,
	};

public:
	COEUIRenderSystem_Impl();
	virtual ~COEUIRenderSystem_Impl();

	virtual void SetTexture(IOETexture* pTexture);
	virtual IOETexture* GetTexture() const;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void FlushAll();

private:
	bool Init();
	void Destroy();

	bool Create();

private:
	IOETexture* m_pTexture;
	IOEShader* m_pShader;

	COEUIVertexCache* m_pVertsCache[VERTEX_CACHE_COUNT];
	bool m_bInitialized;

};
#endif // __OEUIRENDERSYSTEM_IMPL_H__
