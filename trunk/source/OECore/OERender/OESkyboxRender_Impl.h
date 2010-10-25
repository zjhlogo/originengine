/*!
 * \file OESkyboxRender_Impl.h
 * \date 10-25-2010 13:53:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OESKYBOXRENDER_IMPL_H__
#define __OESKYBOXRENDER_IMPL_H__

#include <OECore/IOERender.h>

class COESkyboxRender_Impl : public IOERender
{
public:
	RTTI_DEF(COESkyboxRender_Impl, IOERender);

	COESkyboxRender_Impl();
	virtual ~COESkyboxRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool Init();
	void Destroy();

};
#endif // __OESKYBOXRENDER_IMPL_H__
