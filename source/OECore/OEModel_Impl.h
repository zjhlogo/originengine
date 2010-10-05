/*!
 * \file OEModel_Impl.h
 * \date 9-8-2009 17:49:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODEL_IMPL_H__
#define __OEMODEL_IMPL_H__

#include <OECore/IOEModel.h>
#include <OECore/IOEControl.h>
#include <OECore/IOERender.h>
#include <OEBase/IOEXmlNode.h>
#include <vector>

#include "OESkinMeshRenderData_Impl.h"

class COEModel_Impl : public IOEModel
{
public:
	typedef std::vector<IOEControl*> TV_CONTROL;
	typedef std::vector<IOERender*> TV_RENDER;

public:
	COEModel_Impl(const tstring& strFile);
	virtual ~COEModel_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOEMesh* GetMesh();

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFile);
	bool CreateRenderData(IOEXmlNode* pXmlRoot);

private:
	COESkinMeshRenderData_Impl* m_pRenderData;
	TV_CONTROL m_vControls;
	TV_RENDER m_vRenders;

};

#endif // __OEMODEL_IMPL_H__
