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
#include <libOEMsg/OEMsgCommand.h>
#include "OEUIVertexCache.h"

class COEUIRenderSystem_Impl : public IOEUIRenderSystem
{
public:
	enum CONST_DEFINE
	{
		SOLID_VERTEX_CACHE = 4096*32,
		SOLID_INDEX_CACHE = (4096*3)/2+2,
		NUM_SOLID_CACHE = 5,

		TRANSPARENT_VERTEX_CACHE = 4096*32,
		TRANSPARENT_INDEX_CACHE = (4096*3)/2+2,
		NUM_TRANSPARENT_CACHE = 5,
	};

public:
	RTTI_DEF(COEUIRenderSystem_Impl, IOEUIRenderSystem);

	COEUIRenderSystem_Impl();
	virtual ~COEUIRenderSystem_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual COEUIScreen* GetScreen();

	virtual void SetTexture(IOETexture* pTexture);
	virtual IOETexture* GetTexture() const;

	virtual void DrawSolidTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual void DrawTransparentTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis);
	virtual float NextDepth();

private:
	bool Init();
	void Destroy();

	void FlushSolid();
	void FlushTransparent();
	void FlushAll();

	bool OnUpdate(COEMsgCommand& msg);
	bool OnRender(COEMsgCommand& msg);
	bool OnPostRender(COEMsgCommand& msg);

private:
	IOETexture* m_pTexture;
	IOEShader* m_pShader;

	COEUIVertexCache* m_pSolidVertsCache[NUM_SOLID_CACHE];
	COEUIVertexCache* m_pTransparentVertsCache[NUM_TRANSPARENT_CACHE];
	COEUIScreen* m_pScreen;

	float m_fCurrDepth;

};
#endif // __OEUIRENDERSYSTEM_IMPL_H__
