/*!
 * \file OEWaterRender_Impl.cpp
 * \date 10-27-2010 22:32:40
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "OEWaterRender_Impl.h"
#include <OECore/IOECore.h>
#include <OECore/IOERenderSystem.h>
#include <OECore/OERenderSystemUtil.h>
#include <libOEMsg/OEMsgList.h>

COEWaterRender_Impl::COEWaterRender_Impl()
{
	// TODO: 
}

COEWaterRender_Impl::~COEWaterRender_Impl()
{
	// TODO: 
}

bool COEWaterRender_Impl::Render(IOERenderData* pRenderData)
{
	if (!CreateReflectTexture(pRenderData->GetRenderTarget())) return false;

	IOEMesh* pMesh = pRenderData->GetMesh();
	if (!pMesh) return false;

	int nNumPiece = pMesh->GetNumPieces();

	CMatrix4x4 matModelToWorld = pRenderData->GetNode()->GetFinalMatrix();

	CMatrix4x4 matWorldToProject = matModelToWorld;
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW_PROJ);

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

		//// give user chance to setup shader parameter
		//COEMsgShaderParam msg(pShader);
		//pRenderData->GetHolder()->CallEvent(msg);

		pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);
		pShader->SetTexture(TS("g_texDiffuse"), pRenderData->GetRenderTarget());

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}

bool COEWaterRender_Impl::CreateReflectTexture(IOETexture* pTexture)
{
	if (g_pOECore->IsRenderingSceneToTexture()) return false;

	g_pOERenderSystem->RegisterEvent(OMI_GET_TRANSFORM, this, (MSG_FUNC)&COEWaterRender_Impl::OnGetTransform);
	g_pOERenderSystem->RegisterEvent(OMI_RESTORE_RENDER_STATE, this, (MSG_FUNC)&COEWaterRender_Impl::OnRestoreRenderState);

	CalcuteClipPlane();
	bool bOK = g_pOECore->RenderSceneToTexture(pTexture);

	g_pOERenderSystem->UnregisterEvent(OMI_RESTORE_RENDER_STATE, this, (MSG_FUNC)&COEWaterRender_Impl::OnRestoreRenderState);
	g_pOERenderSystem->UnregisterEvent(OMI_GET_TRANSFORM, this, (MSG_FUNC)&COEWaterRender_Impl::OnGetTransform);

	return bOK;
}

void COEWaterRender_Impl::CalcuteClipPlane()
{
	// world space clip plane
	CMatrix4x4 matWorldToProj;
	g_pOERenderSystem->GetTransform(matWorldToProj, TT_WORLD_VIEW_PROJ);
	CMatrix4x4 matProjToWorld = matWorldToProj.Inverse();
	matProjToWorld = matProjToWorld.Transpose();
	CVector4 vPlane(0.0f, 1.0f, 0.0f, 0.0f);
	m_vClipPlaneWorldSpace = vPlane*matProjToWorld;

	// model space clip plane
	matWorldToProj.Identity();
	g_pOERenderSystem->GetTransform(matWorldToProj, TT_WORLD_VIEW);
	COEMath::SetMatrixTranslation(matWorldToProj, COEMath::VECTOR_ZERO);
	g_pOERenderSystem->GetTransform(matWorldToProj, TT_PROJECTION);

	matProjToWorld = matWorldToProj.Inverse();
	matProjToWorld = matProjToWorld.Transpose();
	m_vClipPlaneModelSpace = vPlane*matProjToWorld;
}

bool COEWaterRender_Impl::OnGetTransform(COEMsgTransform& msg)
{
	if (msg.GetType() == TT_WORLD)
	{
		// set reflect matrix
		CMatrix4x4 matReflect;
		matReflect.m[5] = -1.0f;
		matReflect *= msg.GetMatrix();
		msg.SetMatrix(matReflect);
	}

	return true;
}

bool COEWaterRender_Impl::OnRestoreRenderState(COEMsgRenderState& msg)
{
	COERenderState& RenderState = msg.GetRenderState();

	RenderState.m_bEnableClipPlane = true;
	if (msg.GetCommon() == TS("ModelSpace"))
	{
		RenderState.m_vClipPlane = m_vClipPlaneModelSpace;
	}
	else
	{
		RenderState.m_vClipPlane = m_vClipPlaneWorldSpace;
	}

	if (RenderState.m_eCullMode == CMT_CW)
	{
		RenderState.m_eCullMode = CMT_CCW;
	}
	else if (RenderState.m_eCullMode == CMT_CCW)
	{
		RenderState.m_eCullMode = CMT_CW;
	}

	return true;
}
