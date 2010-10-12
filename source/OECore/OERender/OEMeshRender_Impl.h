/*!
 * \file OEMeshRender_Impl.h
 * \date 10-12-2010 17:53:58
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMESHRENDER_IMPL_H__
#define __OEMESHRENDER_IMPL_H__

#include <OECore/IOERender.h>

class COEMeshRender_Impl : public IOERender
{
public:
	RTTI_DEF(COEMeshRender_Impl, IOERender);

	COEMeshRender_Impl();
	virtual ~COEMeshRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

};
#endif // __OEMESHRENDER_IMPL_H__
