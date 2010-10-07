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
#include <OECore/IOERenderableObject.h>

class CSimpleShape : public IOERenderableObject
{
public:
	enum CONST_DEFINE
	{
		CUBE_VERTS_COUNT = 8,
		CUBE_INDIS_COUNT = 36,
	};

	enum SHAPE
	{
		S_UNKNOWN = 0,
		S_CUBE,
		S_SPHERE,
	};

public:
	RTTI_DEF(CSimpleShape, IOERenderableObject);

	CSimpleShape();
	virtual ~CSimpleShape();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOERenderData* GetRenderData();

	void SetShape(SHAPE eShape);
	SHAPE GetShape();

	void SetPosition(const CVector3& vPos);
	const CVector3& GetPosition();

	void SetScale(float fScale);
	float GetScale();

	void SetColor(uint nColor);
	uint GetColor();

private:
	bool Init();
	void Destroy();

	void DrawCube();

private:
	IOEShader* m_pShader;
	SHAPE m_eShape;

	CVector3 m_vPos;
	float m_fScale;
	uint m_nColor;

};

#endif // __SIMPLESHAPE_H__
