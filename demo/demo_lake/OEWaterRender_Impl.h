/*!
 * \file OEWaterRender_Impl.h
 * \date 10-27-2010 22:31:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEWATERRENDER_IMPL_H__
#define __OEWATERRENDER_IMPL_H__

#include <OECore/IOERender.h>
#include <libOEMsg/OEMsgTransform.h>
#include <libOEMsg/OEMsgRenderState.h>

class COEWaterRender_Impl : public IOERender
{
public:
	RTTI_DEF(COEWaterRender_Impl, IOERender);

	COEWaterRender_Impl();
	virtual ~COEWaterRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool CreateReflectTexture(IOETexture* pTexture);
	void CalcuteClipPlane();

	bool OnGetTransform(COEMsgTransform& msg);
	bool OnRestoreRenderState(COEMsgRenderState& msg);

private:
	CVector4 m_vClipPlaneWorldSpace;
	CVector4 m_vClipPlaneModelSpace;

};
#endif // __OEWATERRENDER_IMPL_H__
