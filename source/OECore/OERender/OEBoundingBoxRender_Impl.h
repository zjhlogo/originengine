/*!
 * \file OEBoundingBoxRender_Impl.h
 * \date 2-3-2010 9:12:12
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEBOUNDINGBOXRENDER_IMPL_H__
#define __OEBOUNDINGBOXRENDER_IMPL_H__

#include <OECore/IOERender.h>
#include <OECore/IOEShader.h>

class COEBoundingBoxRender_Impl : public IOERender
{
public:
	RTTI_DEF(COEBoundingBoxRender_Impl, IOERender);

	COEBoundingBoxRender_Impl();
	virtual ~COEBoundingBoxRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool Init();
	void Destroy();

private:
	IOEShader* m_pShader;

};
#endif // __OEBOUNDINGBOXRENDER_IMPL_H__
