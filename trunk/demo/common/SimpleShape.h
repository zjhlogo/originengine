/*!
 * \file SimpleShape.h
 * \date 6-7-2009 11:22:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __SIMPLESHAPE_H__
#define __SIMPLESHAPE_H__

#include <OEInterfaces.h>

class CSimpleShape
{
public:
	typedef struct VERTEX_tag
	{
		float x, y, z;
		uint nColor;
	} VERTEX;

public:
	CSimpleShape();
	~CSimpleShape();

	bool Initialize();
	void DrawCube(IOERenderSystem* pRenderer, const CVector3& vPos, float fScale = 1.0f, uint nColor = 0xFFFFFFFF);

private:
	void Init();
	void Destroy();

private:
	IOEVertDecl* m_pDecl;
	bool m_bOK;

};

#endif // __SIMPLESHAPE_H__
