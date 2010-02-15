/*!
 * \file OESkelectonRender_Impl.h
 * \date 14-2-2010 8:50:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OESKELECTONRENDER_IMPL_H__
#define __OESKELECTONRENDER_IMPL_H__

#include <IOERender.h>
#include <IOEShader.h>
#include <vector>

#include "OESkinMeshRenderData_Impl.h"

class COESkelectonRender_Impl : public IOERender
{
public:
	typedef struct VERTEX_tag
	{
		float x, y, z;
		uint color;
	} VERTEX;

	typedef std::vector<VERTEX> TV_VERTEX;
	typedef std::vector<ushort> TV_SHORT;

public:
	COESkelectonRender_Impl();
	virtual ~COESkelectonRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	void Init();
	void Destroy();

	COESkinMeshRenderData_Impl* ConvertData(IOERenderData* pRenderData);
	bool BuildBoneVerts(TV_VERTEX& vVertsOut, TV_SHORT& vIndisOut, const TV_BONE& vBones, const TV_MATRIX& vmatSkins, int nBoneID, int nParentBoneID);

private:
	TV_VERTEX m_vVerts;
	TV_SHORT m_vIndis;
	IOEShader* m_pShader;

};
#endif // __OESKELECTONRENDER_IMPL_H__
