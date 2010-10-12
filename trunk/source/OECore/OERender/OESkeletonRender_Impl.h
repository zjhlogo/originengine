/*!
 * \file OESkeletonRender_Impl.h
 * \date 1-3-2010 19:23:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKELETONRENDER_IMPL_H__
#define __OESKELETONRENDER_IMPL_H__

#include <OECore/OEBaseTypeEx.h>
#include <OECore/IOERender.h>
#include <OECore/IOEShader.h>

class COESkeletonRender_Impl : public IOERender
{
public:
	RTTI_DEF(COESkeletonRender_Impl, IOERender);

	COESkeletonRender_Impl();
	virtual ~COESkeletonRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool Init();
	void Destroy();

	bool BuildBoneVerts(TV_VERTEX_LINE& vVertsOut, TV_VERTEX_INDEX& vIndisOut, IOESkeleton* pSkeleton, const TV_MATRIX4X4& vmatSkins, int nBoneID, int nParentBoneID);

private:
	TV_VERTEX_LINE m_vVerts;
	TV_VERTEX_INDEX m_vIndis;
	IOEShader* m_pShader;

};
#endif // __OESKELETONRENDER_IMPL_H__
