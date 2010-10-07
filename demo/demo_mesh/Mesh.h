/*!
 * \file Mesh.h
 * \date 10-6-2010 22:25:57
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __MESH_H__
#define __MESH_H__

#include <OECore/IOERenderableObject.h>
#include <OECore/IOEMesh.h>
#include <OECore/IOETexture.h>
#include <OECore/IOEShader.h>

class CMesh : public IOERenderableObject
{
public:
	RTTI_DEF(CMesh, IOERenderableObject);

	CMesh();
	virtual ~CMesh();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOERenderData* GetRenderData();

	const CVector3& GetBoundingBoxMin();
	const CVector3& GetBoundingBoxMax();

private:
	bool Init();
	void Destroy();

private:
	IOEMesh* m_pMesh;
	IOETexture* m_pTexture;
	IOEShader* m_pShader;

};
#endif // __MESH_H__
