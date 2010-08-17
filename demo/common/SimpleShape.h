/*!
 * \file SimpleShape.h
 * \date 6-7-2009 11:22:09
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __SIMPLESHAPE_H__
#define __SIMPLESHAPE_H__

#include <OECore/IOEShader.h>

class CSimpleShape
{
public:
	enum CONST_DEFINE
	{
		CUBE_VERTS_COUNT = 8,
		CUBE_INDIS_COUNT = 36,
	};

public:
	CSimpleShape();
	~CSimpleShape();

	bool IsOK();
	void DrawCube(const CVector3& vPos, float fScale = 1.0f, uint nColor = 0xFFFFFFFF);

private:
	bool Init();
	void Destroy();

private:
	bool m_bOK;
	IOEShader* m_pShader;

};

#endif // __SIMPLESHAPE_H__
