/*!
 * \file IOEUIRenderSystem.h
 * \date 19-2-2010 19:49:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEUIRENDERSYSTEM_H__
#define __IOEUIRENDERSYSTEM_H__

#include "IOEMgr.h"
#include "OEUIType.h"
#include "../IOETexture.h"

class IOEUIRenderSystem : public IOEMgr
{
public:
	IOEUIRenderSystem() {};
	virtual ~IOEUIRenderSystem() {};

	virtual void SetTexture(IOETexture* pTexture) = 0;
	virtual IOETexture* GetTexture() const = 0;

	virtual void DrawTriList(const void* pVerts, uint nVerts, const ushort* pIndis, uint nIndis) = 0;

};

extern IOEUIRenderSystem* g_pOEUIRenderSystem;
#endif // __IOEUIRENDERSYSTEM_H__