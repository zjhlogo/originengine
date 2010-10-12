/*!
 * \file BumpMap.h
 * \date 10-6-2010 21:39:41
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __BUMPMAP_H__
#define __BUMPMAP_H__

#include <OECore/IOERenderableObject.h>
#include <OECore/IOEShader.h>
#include <OECore/IOETexture.h>

class CBumpMap : public IOERenderableObject
{
public:
	typedef struct VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
	} VERTEX;

public:
	RTTI_DEF(CBumpMap, IOERenderableObject);

	CBumpMap();
	virtual ~CBumpMap();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOERenderData* GetRenderData();

	void SetEyePos(const CVector3& vEyePos);
	const CVector3& GetEyePos();

	void SetLightPos(const CVector3& vLightPos);
	const CVector3& GetLightPos();

	void SetTechnique(const tstring& strTechnique);

private:
	bool Init();
	void Destroy();

private:
	IOEShader* m_pShader;
	IOETexture* m_pTexDiffuse;
	IOETexture* m_pTexNormalHeight;

	CVector3 m_vLightPos;
	CVector3 m_vEyePos;

};
#endif // __BUMPMAP_H__
