/*!
 * \file OEMaterialsList_Impl.h
 * \date 10-29-2010 19:15:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMATERIALSLIST_IMPL_H__
#define __OEMATERIALSLIST_IMPL_H__

#include <OECore/IOEMaterialsList.h>
#include <OEBase/IOEXmlNode.h>
#include <map>

class COEMaterialsList_Impl : public IOEMaterialsList
{
public:
	typedef std::map<int, IOEMaterial*> TM_MATERIAL;

public:
	RTTI_DEF(COEMaterialsList_Impl, IOEMaterialsList);

	COEMaterialsList_Impl(IOEXmlNode* pXmlMaterials);
	virtual ~COEMaterialsList_Impl();

	virtual IOEMaterial* GetMaterial(int nID);
	virtual int GetNumMaterial();

private:
	void Init();
	void Destroy();

	bool Create(IOEXmlNode* pXmlMaterials);

private:
	TM_MATERIAL m_mapMaterials;

};
#endif // __OEMATERIALSLIST_IMPL_H__
