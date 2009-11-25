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

	typedef struct MODEL_VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
		uchar nBoneIndex[4];
		float fWeight[4];
	} MODEL_VERTEX;

	typedef struct NORMAL_VERTEX_tag
	{
		float x, y, z;
		uint nColor;
	} NORMAL_VERTEX;

	typedef std::vector<NORMAL_VERTEX> TV_NORMAL_VERTEX;
	typedef std::vector<ushort> TV_USHORT;

public:
	CMeshApp();
	virtual ~CMeshApp();

	virtual bool Initialize();
	virtual void Terminate();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool OnLButtonDown(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnLButtonUp(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnMouseMove(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnKeyUp(uint nMsgID, COEDataBufferRead* pDBRead);
	bool OnKeyDown(uint nMsgID, COEDataBufferRead* pDBRead);

	bool UpdateMovement(float fDetailTime);
	bool UpdateRotation(float fDetailTime);

	void RenderPieceNormal(IOEPiece* pPiece);

private:
	CCamera* m_pCamera;
	CSimpleShape* m_pSimpleShape;

	IOEModel* m_pModel;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

	IOEVertDecl* m_pNormalVertDecl;
	TV_NORMAL_VERTEX m_vNormalVerts;
	TV_USHORT m_vNormalIndis;

};

#endif // __MESHAPP_H__
