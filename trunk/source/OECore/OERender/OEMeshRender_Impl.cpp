/*!
 * \file OEMeshRender_Impl.cpp
 * \date 10-12-2010 17:54:02
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEMeshRender_Impl.h"
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEMsg/OEMsgShaderParam.h>

COEMeshRender_Impl::COEMeshRender_Impl()
{
	// TODO: 
}

COEMeshRender_Impl::~COEMeshRender_Impl()
{
	// TODO: 
}

bool COEMeshRender_Impl::Render(IOERenderData* pRenderData)
{
	IOEMesh* pMesh = pRenderData->GetMesh();
	if (!pMesh) return false;

	int nNumPiece = pMesh->GetNumPieces();

	CMatrix4x4 matWorld = pRenderData->GetNode()->GetFinalMatrix();

	CMatrix4x4 matWorldViewProj = matWorld;
	g_pOERenderSystem->GetTransform(matWorldViewProj, TT_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);
		if (!pPiece) continue;

		IOEMaterial* pMaterial = pRenderData->GetMaterial(pPiece->GetMaterialID());
		if (!pMaterial) continue;

		IOEShader* pShader = pMaterial->GetShader();
		if (!pShader) continue;

		if (pPiece->GetVertDeclMask() != pMaterial->GetVertDeclMask()) continue;

		// give user chance to setup shader parameter
		COEMsgShaderParam msg(pShader);
		pRenderData->GetHolder()->CallEvent(msg);

		pShader->SetMatrix(TS("g_matWorldViewProj"), matWorldViewProj);
		pShader->SetTexture(TS("g_texDiffuse"), pMaterial->GetTexture(MTT_DIFFUSE));

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}
