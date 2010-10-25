/*!
 * \file OESkyboxRender_Impl.cpp
 * \date 10-25-2010 13:56:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OESkyboxRender_Impl.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <libOEMsg/OEMsgShaderParam.h>

COESkyboxRender_Impl::COESkyboxRender_Impl()
{
	m_bOK = Init();
}

COESkyboxRender_Impl::~COESkyboxRender_Impl()
{
	Destroy();
}

bool COESkyboxRender_Impl::Init()
{
	// TODO: 
	return true;
}

void COESkyboxRender_Impl::Destroy()
{
	// TODO: 
}

bool COESkyboxRender_Impl::Render(IOERenderData* pRenderData)
{
	IOEMesh* pMesh = pRenderData->GetMesh();
	if (!pMesh) return false;

	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return false;

	const CVector3& vEyePos = pCameraNode->GetPosition();
	CMatrix4x4 matWorld;
	COEMath::SetMatrixTranslation(matWorld, vEyePos);

	CMatrix4x4 matWorldToProject = matWorld;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_VIEW_PROJ);

	CDefaultRenderState DefaultState;

	g_pOERenderSystem->EnableZBuffer(false);

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
		COEMsgShaderParam msg(pShader);
		pRenderData->GetHolder()->CallEvent(msg);

		pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);
		pShader->SetTexture(TS("g_texDiffuse"), pMaterial->GetTexture(MTT_DIFFUSE));

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}
