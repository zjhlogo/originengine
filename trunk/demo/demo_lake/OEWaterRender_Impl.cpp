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
#include <OECore/IOEDevice.h>
#include <libOEMsg/OEMsgList.h>
#include <libOEMsg/OEMsgShaderParam.h>

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
	static float s_fTotalTime = 0.0f;

	s_fTotalTime += g_pOEDevice->GetDetailTime();
	if (s_fTotalTime > (COEMath::PIX2*100.0f)) s_fTotalTime -= (COEMath::PIX2*100.0f);

	IOEMesh* pMesh = pRenderData->GetMesh(TS("MainMesh"));
	if (!pMesh) return false;

	IOENode* pAttachedNode = (IOENode*)pRenderData->GetObject(TS("AttachedNode"));
	if (!pAttachedNode) return false;

	IOEMaterialsList* pMaterialsList = pRenderData->GetMaterialsList(TS("MainMaterialsList"));
	if (!pMaterialsList) return false;

	IOETexture* pRenderTargetRefract = pRenderData->GetTexture(TS("RenderTargetRefract"));
	if (!pRenderTargetRefract) return false;

	IOETexture* pRenderTargetReflect = pRenderData->GetTexture(TS("RenderTargetReflect"));
	if (!pRenderTargetReflect) return false;

	IOENode* pCameraNode = g_pOECore->GetRootNode()->GetChildNode(TS("Camera"));
	if (!pCameraNode) return false;

	if (!CreateRefractTexture(pRenderData, pRenderTargetRefract)) return false;

	if (!CreateReflectTexture(pRenderTargetReflect)) return false;

	CMatrix4x4 matWorldToProject = pAttachedNode->GetFinalMatrix();
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

		//// give user chance to setup shader parameter
		//IOEObject* pHolder = pRenderData->GetObject(TS("Holder"));
		//if (pHolder)
		//{
		//	COEMsgShaderParam msg(pShader, pMaterial);
		//	pHolder->CallEvent(msg);
		//}

		pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);
		pShader->SetTexture(TS("g_texReflect"), pRenderTargetReflect);
		pShader->SetTexture(TS("g_texRefract"), pRenderTargetRefract);
		pShader->SetTexture(TS("g_texNormal"), pMaterial->GetTexture(MTT_NORMAL));
		pShader->SetVector(TS("g_vEyePos"), pCameraNode->GetPosition());
		pShader->SetFloat(TS("g_fTime"), s_fTotalTime);

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return true;
}

bool COEWaterRender_Impl::CreateRefractTexture(IOERenderData* pRenderData, IOETexture* pTexture)
{
	IOEMesh* pMesh = pRenderData->GetMesh(TS("MainMesh"));
	if (!pMesh) return false;

	IOENode* pAttachedNode = (IOENode*)pRenderData->GetObject(TS("AttachedNode"));
	if (!pAttachedNode) return false;

	IOEMaterialsList* pMaterialsList = pRenderData->GetMaterialsList(TS("MainMaterialsList"));
	if (!pMaterialsList) return false;

	CMatrix4x4 matWorldToProject = pAttachedNode->GetFinalMatrix();
	g_pOERenderSystem->GetTransform(matWorldToProject, TT_WORLD_VIEW_PROJ);

	CDefaultRenderState DefaultState;
	g_pOERenderSystem->SetColorWriteChannel(CWC_ALPHA);

	int nNumPiece = pMesh->GetNumPieces();
	for (int i = 0; i < nNumPiece; ++i)
	{
		IOEPiece* pPiece = pMesh->GetPiece(i);
		if (!pPiece) continue;

		IOEMaterial* pMaterial = pMaterialsList->GetMaterial(1);		// TODO: material id 
		if (!pMaterial) continue;

		IOEShader* pShader = pMaterial->GetShader();
		if (!pShader) continue;

		if (pPiece->GetVertDeclMask() != pMaterial->GetVertDeclMask()) continue;

		//// give user chance to setup shader parameter
		//IOEObject* pHolder = pRenderData->GetObject(TS("Holder"));
		//if (pHolder)
		//{
		//	COEMsgShaderParam msg(pShader, pMaterial);
		//	pHolder->CallEvent(msg);
		//}

		pShader->SetMatrix(TS("g_matWorldToProject"), matWorldToProject);

		g_pOERenderSystem->SetShader(pShader);
		g_pOERenderSystem->DrawTriList(pPiece->GetVerts(), pPiece->GetNumVerts(), pPiece->GetIndis(), pPiece->GetNumIndis());
	}

	return g_pOERenderSystem->CopyBackbuffer(pTexture);
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
