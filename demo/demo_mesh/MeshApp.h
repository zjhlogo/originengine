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

	typedef std::vector<VERTEX> VVERTEX;
	typedef std::vector<ushort> VUSHORT;

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

private:
	CCamera* m_pCamera;
	CSimpleShape* m_pSimpleShape;

	IOEVertDecl* m_pDecl;
	IOEMesh* m_pMesh;

	VVERTEX m_vVerts;
	VUSHORT m_vIndis;

	bool m_bLButtonDown;
	int m_nMouseDetailX;
	int m_nMouseDetailY;
	bool m_KeyDown[KEY_COUNT];

};

#endif // __MESHAPP_H__
