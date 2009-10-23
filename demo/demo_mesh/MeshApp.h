/*!
 * \file MeshApp.h
 * \date 31-7-2009 20:41:47
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MESHAPP_H__
#define __MESHAPP_H__

#include <IOEApp.h>
#include <OEInterfaces.h>

#include "../common/Camera.h"
#include "../common/SimpleShape.h"

#include <vector>

class CMeshApp : public IOEApp
{
public:
	enum CONST_DEFINE
	{
		KEY_COUNT = 256,
	};

	typedef struct VERTEX_tag
	{
		float x, y, z;
		uint nColor;
	} VERTEX;

	typedef struct VERTEX_PIECE_tag
	{
		float x, y, z;
		float u, v;
		int nBoneIndex[4];
		float fBoneWeight[4];
	} VERTEX_PIECE;

	typedef std::vector<VERTEX> VVERTEX;
	typedef std::vector<ushort> VUSHORT;
	typedef std::vector<CMatrix4x4> TV_MATRIX;

public:
	CMeshApp();
	virtual ~CMeshApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual void OnLButtonDown(int x, int y);
	virtual void OnLButtonUp(int x, int y);
	virtual void OnMouseMove(int dx, int dy);
	virtual void OnKeyUp(int nKeyCode);
	virtual void OnKeyDown(int nKeyCode);

private:
	void Init();
	void Destroy();

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

	//void RebuildBoneVerts(float fTime);
	//void BuildBoneVerts(VVERTEX& vVerts, VUSHORT& vIndis, float fTime, IOEMeshBone* pBone, const CMatrix4x4& matParent, int nParentVertIndex);

private:
	CCamera* m_pCamera;
	CSimpleShape* m_pSimpleShape;

	IOEVertDecl* m_pDecl;
	IOEVertDecl* m_pDecl2;
	IOEModel* m_pModel;

	IOEShader* m_pShader;
	IOETexture* m_pTexBase;

	VVERTEX m_vVerts;
	VUSHORT m_vIndis;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

	TV_MATRIX m_matBones;

};

#endif // __MESHAPP_H__
