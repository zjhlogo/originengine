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

class COEModel_Impl : public IOEModel
{
public:
	typedef std::vector<IOEControl*> TV_CONTROL;
	typedef std::vector<IOERender*> TV_RENDER;

public:
	RTTI_DEF(COEModel_Impl, IOEModel);

	COEModel_Impl(const tstring& strFile);
	virtual ~COEModel_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOERenderData* GetRenderData();

	virtual bool AddControl(const tstring& strControl);
	virtual void RemoveControl(const tstring& strControl);

	virtual bool AddRender(const tstring& strRender);
	virtual void RemoveRender(const tstring& strRender);

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFile);

	bool CreateRenderData(IOEXmlNode* pXmlRenderData);
	bool CreateControls(IOEXmlNode* pXmlControls);
	bool CreateRenderers(IOEXmlNode* pXmlRenderers);

	bool LoadMesh(const tstring& strFile);
	bool LoadAnimData(const tstring& strFile);
	bool LoadMaterialsList(IOEXmlNode* pXmlMaterials);

	bool CreateMesh(const tstring& strFile);
	bool CreateAnimData(const tstring& strFile);
	bool CreateMaterialsList(IOEXmlNode* pXmlMaterials);

	void DestroyMesh();
	void DestroyAnimData();
	void DestroyMaterialsList();

private:
	IOERenderData* m_pRenderData;
	TV_CONTROL m_vControls;
	TV_RENDER m_vRenders;

};

#endif // __OEMODEL_IMPL_H__
