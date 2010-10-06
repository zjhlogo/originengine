/*!
 * \file OEMaterial_Impl.h
 * \date 1-3-2010 19:33:29
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMATERIAL_IMPL_H__
#define __OEMATERIAL_IMPL_H__

#include <OECore/OEBaseTypeEx.h>
#include <OECore/IOEMaterial.h>
#include <OEBase/IOEXmlNode.h>

class COEMaterial_Impl : public IOEMaterial
{
public:
	RTTI_DEF(COEMaterial_Impl, IOEMaterial);

	COEMaterial_Impl(IOEXmlNode* pXmlMaterial);
	virtual ~COEMaterial_Impl();

	virtual int GetID();
	virtual int GetVertDeclMask();
	virtual IOEShader* GetShader();
	virtual IOETexture* GetTexture(MATERIAL_TEXTURE_TYPE eType);

private:
	void Init();
	void Destroy();

	bool Create(IOEXmlNode* pXmlMaterial);
	const tstring& GetTextureKey(MATERIAL_TEXTURE_TYPE eType);
	IOEShader* CreateShaderFromVertDecl(int nVertDeclMask, const tstring& strFile);

private:
	int m_nID;
	int m_nVertDeclMask;

	tstring m_strShader;
	IOEShader* m_pShader;

	tstring m_strTexture[MTT_NUM];
	IOETexture* m_pTexture[MTT_NUM];

};
#endif // __OEMATERIAL_IMPL_H__
