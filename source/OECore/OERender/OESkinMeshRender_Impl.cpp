/*!
 * \file OESkinMeshRender_Impl.cpp
 * \date 13-2-2010 19:58:25
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "OESkinMeshRender_Impl.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <OEBase/IOEMsgMgr.h>
#include <libOEMsg/OEMsgList.h>
#include <libOEMsg/OEMsgShaderParam.h>

COESkinMeshRender_Impl::COESkinMeshRender_Impl()
{
	// TODO: 
}

COESkinMeshRender_Impl::~COESkinMeshRender_Impl()
{
	// TODO: 
}

bool COESkinMeshRender_Impl::Render(IOERenderData* pRenderData)
{
	IOEMesh* pMesh = pRenderData->GetMesh();
	if (!pMesh) return false;

	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return false;

	CMatrix4x4 matModelToWorld = pRenderData->GetNode()->GetFinalMatrix();
	CMatrix4x4 matWorldToProject = matModelToWorld;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	int nNumPiece = pMesh->GetNumPieces();
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
		COEMsgShaderParam msgShaderParam(pShader);
		pRenderData->GetHolder()->CallEvent(msgShaderParam);

		pShader->SetMatrix(TS("g_matWorldToModel"), matModelToWorld.Inverse());
		pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);
		pShader->SetTexture(TS("g_texDiffuse"), pMaterial->GetTexture(MTT_DIFFUSE));
		pShader->SetTexture(TS("g_texNormal"), pMaterial->GetTexture(MTT_NORMAL));
		pShader->SetVector(TS("g_vEyePos"), pCameraNode->GetPosition());
		
		const TV_MATRIX4X4& vmatSkins = pRenderData->GetSkinMatrix();
		if (vmatSkins.size() <= 0) continue;

		pShader->SetMatrixArray(TS("g_matBoneMatrix"), &vmatSkins[0], vmatSkins.size());

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}
