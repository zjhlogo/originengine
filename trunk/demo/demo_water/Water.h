/*!
 * \file Water.h
 * \date 10-6-2010 22:58:29
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __WATER_H__
#define __WATER_H__

#include <OECore/IOERenderableObject.h>
#include <OECore/IOEShader.h>

class CWater : public IOERenderableObject
{
public:
	enum CONST_DEFINE
	{
		NUM_X = 80,
		NUM_Z = 80,
	};

	typedef struct VERTEX_tag
	{
		float x, y, z;
		float u, v;
	} VERTEX;

public:
	RTTI_DEF(CWater, IOERenderableObject);

	CWater();
	virtual ~CWater();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOERenderData* GetRenderData();

	void SetTime(float fTime) {m_fTime = fTime;};
	void SetVecFreq(const CVector4& vecFreq) {m_vVecFreq = vecFreq;};
	void SetVecSpeed(const CVector4& vecSpeed) {m_vVecSpeed = vecSpeed;};
	void SetVecDirX(const CVector4& vecDirX) {m_vVecDirX = vecDirX;};
	void SetVecDirY(const CVector4& vecDirY) {m_vVecDirY = vecDirY;};
	void SetVecHeight(const CVector4& vecHeight) {m_vVecHeight = vecHeight;};
	void SetEyePos(const CVector3& vEyePos) {m_vEyePos = vEyePos;};

private:
	bool Init();
	void Destroy();

private:
	VERTEX* m_pVerts;
	ushort* m_pIndis;

	uint m_nVerts;
	uint m_nIndis;

	IOEShader* m_pShader;

	float m_fTime;
	CVector4 m_vVecFreq;
	CVector4 m_vVecSpeed;
	CVector4 m_vVecDirX;
	CVector4 m_vVecDirY;
	CVector4 m_vVecHeight;
	CVector3 m_vEyePos;

};
#endif // __WATER_H__
