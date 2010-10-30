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
#include <OECore/IOENode.h>
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
	IOEMesh* pMesh = pRenderData->GetMesh(TS("MainMesh"));
	if (!pMesh) return false;

	IOEMaterialsList* pMaterialsList = pRenderData->GetMaterialsList(TS("MainMaterialsList"));
	if (!pMaterialsList) return false;

	CMatrix4x4 matWorldToProject;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW);
	COEMath::SetMatrixTranslation(matWorldToProject, COEMath::VECTOR_ZERO);
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_PROJECTION);

	CDefaultRenderState DefaultState(TS("ModelSpace"));

	g_pOERenderSystem->EnableZBuffer(false);

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

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}
