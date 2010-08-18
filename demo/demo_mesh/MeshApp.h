/*!
 * \file MeshApp.h
 * \date 31-7-2009 20:41:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MESHAPP_H__
#define __MESHAPP_H__

#include "../common/BaseApp.h"
#include <OECore/IOEPiece.h>
#include <OECore/IOEMesh.h>
#include <OECore/IOETexture.h>
#include <OECore/IOEShader.h>

class CMeshApp : public CBaseApp
{
public:
	CMeshApp();
	virtual ~CMeshApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnSetupShaderParam(COEMsgShaderParam& msg);
	void RenderPieceNormal(IOEPiece* pPiece);

private:
	IOEMesh* m_pMesh;
	IOETexture* m_pTexture;
	IOEShader* m_pShader;

};

#endif // __MESHAPP_H__
