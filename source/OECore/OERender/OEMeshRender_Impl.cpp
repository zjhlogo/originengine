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
#include <OECore/IOENode.h>
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
	IOEMesh* pMesh = pRenderData->GetMesh(TS("MainMesh"));
	if (!pMesh) return false;

	IOEMaterialsList* pMaterialsList = pRenderData->GetMaterialsList(TS("MainMaterialsList"));
	if (!pMaterialsList) return false;

	IOENode* pAttachedNode = (IOENode*)pRenderData->GetObject(TS("AttachedNode"));
	if (!pAttachedNode) return false;

	CMatrix4x4 matModelToWorld = pAttachedNode->GetFinalMatrix();
	CMatrix4x4 matWorldToProject = matModelToWorld;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	int nNumPiece = pMesh->GetNumPieces();
	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);
		if (!pPiece) continue;

		IOEMaterial* pMaterial = pMaterialsList->GetMaterial(pPiece->GetMaterialID());
		if (!pMaterial) continue;

		IOEShader* pShader = pMaterial->GetShader();
		if (!pShader) continue;

		if (pPiece->GetVertDeclMask() != pMaterial->GetVertDeclMask()) continue;

		// give user chance to setup shader parameter
		IOEObject* pHolder = pRenderData->GetObject(TS("Holder"));
		if (pHolder)
		{
			COEMsgShaderParam msg(pShader);
			pHolder->CallEvent(msg);
		}

		pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);
		pShader->SetTexture(TS("g_texDiffuse"), pMaterial->GetTexture(MTT_DIFFUSE));
		pShader->SetMatrix(TS("g_matWorldToModel"), matModelToWorld.Inverse());

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}
