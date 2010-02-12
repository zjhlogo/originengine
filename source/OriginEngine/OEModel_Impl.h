/*!
 * \file OEModel_Impl.h
 * \date 9-8-2009 17:49:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODEL_IMPL_H__
#define __OEMODEL_IMPL_H__

#include <IOEModel.h>
#include <IOERenderData.h>
#include <IOEControl.h>
#include <IOERender.h>
#include <vector>

class COEModel_Impl : public IOEModel
{
public:
	typedef std::vector<IOEControl*> TV_CONTROL;

public:
	COEModel_Impl(const tstring& strFile);
	virtual ~COEModel_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render();

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFile);

private:
	IOERenderData* m_pRenderData;
	TV_CONTROL m_vControls;
	IOERender* m_pRender;

};

#endif // __OEMODEL_IMPL_H__
