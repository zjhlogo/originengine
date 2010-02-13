/*!
 * \file OESkinMeshRender_Impl.cpp
 * \date 13-2-2010 19:58:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkinMeshRender_Impl.h"
#include <IOERenderSystem.h>

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

	const TV_MATERIAL& vMaterials = pData->GetMaterials();
	const TV_MATRIX& vmatSkins = pData->GetSkinMatrix();

	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);

		int nMaterialID = pPiece->GetMaterialID();
		if (nMaterialID >= (int)vMaterials.size()) continue;
		if (pPiece->GetVertDecl() != vMaterials[nMaterialID].nVertDecl) continue;

		vMaterials[nMaterialID].pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
		vMaterials[nMaterialID].pShader->SetTexture(TS("g_texBase"), vMaterials[nMaterialID].pTexture);
		vMaterials[nMaterialID].pShader->SetTexture(TS("g_texNormal"), vMaterials[nMaterialID].pTexNormal);
		vMaterials[nMaterialID].pShader->SetMatrixArray(TS("g_matBoneMatrix"), &vmatSkins[0], vmatSkins.size());
		g_pOERenderSystem->SetShader(vMaterials[nMaterialID].pShader);

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
