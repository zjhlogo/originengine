/*!
 * \file OESkinMeshRender_Impl.cpp
 * \date 13-2-2010 19:58:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkinMeshRender_Impl.h"
#include <IOERenderSystem.h>
#include <OERenderSystemUtil.h>
#include <IOEMsgMgr.h>
#include <OEMsgID.h>

COESkinMeshRender_Impl::COESkinMeshRender_Impl()
:IOERender(OERT_SKINMESH)
{
	// TODO: 
}

COESkinMeshRender_Impl::~COESkinMeshRender_Impl()
{
	// TODO: 
}

bool COESkinMeshRender_Impl::Render(IOERenderData* pRenderData)
{
	COESkinMeshRenderData_Impl* pData = ConvertData(pRenderData);
	if (!pData) return false;

	IOEMesh* pMesh = pData->GetMesh();
	if (!pMesh) return false;
	int nNumPiece = pMesh->GetNumPieces();

	CMatrix4x4 matWorldViewProj;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_WORLD_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);
		if (!pPiece) continue;

		IOEMaterial* pMaterial = pData->GetMaterial(pPiece->GetMaterialID());
		if (!pMaterial) continue;

		IOEShader* pShader = pMaterial->GetShader();
		if (!pShader) continue;

		if (pPiece->GetVertDeclMask() != pMaterial->GetVertDeclMask()) continue;

		// give user chance to setup shader parameter
		COEMsg msg(OMI_SETUP_SHADER_PARAM);
		msg.Write(pShader);
		g_pOEMsgMgr->InvokeMessage(&msg);

		pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
		pShader->SetTexture(TS("g_texDiffuse"), pMaterial->GetTexture(MTT_DIFFUSE));
		pShader->SetTexture(TS("g_texNormal"), pMaterial->GetTexture(MTT_NORMAL));

		const TV_MATRIX4X4& vmatSkins = pData->GetSkinMatrix();
		pShader->SetMatrixArray(TS("g_matBoneMatrix"), &vmatSkins[0], vmatSkins.size());

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}

COESkinMeshRenderData_Impl* COESkinMeshRender_Impl::ConvertData(IOERenderData* pRenderData)
{
	if (!pRenderData) return NULL;

	if (pRenderData->GetType() != OERDT_SKINMESH) return NULL;

	return (COESkinMeshRenderData_Impl*)pRenderData;
}
