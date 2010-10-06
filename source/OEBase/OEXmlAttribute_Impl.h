/*!
 * \file OEXmlAttribute_Impl.h
 * \date 11-10-2009 10:05:50
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEXMLATTRIBUTE_IMPL_H__
#define __OEXMLATTRIBUTE_IMPL_H__

#include <OEBase/IOEXmlAttribute.h>

class COEXmlAttribute_Impl : public IOEXmlAttribute
{
public:
	RTTI_DEF(COEXmlAttribute_Impl, IOEXmlAttribute);

	COEXmlAttribute_Impl(const tstring& strName);
	virtual ~COEXmlAttribute_Impl();

	virtual const tstring& GetName();
	virtual void SetName(const tstring& strName);

	virtual const tstring& GetValue();
	virtual void SetValue(const tstring& strValue);

	virtual bool ToString(tstring& strNode);

	void LinkSibling(COEXmlAttribute_Impl* pAttributeSibling);
	COEXmlAttribute_Impl* GetNextSibling();

private:
	bool Init();
	void Destroy();

private:
	tstring m_strName;
	tstring m_strValue;
	COEXmlAttribute_Impl* m_pAttributeSibling;

};
#endif // __OEXMLATTRIBUTE_IMPL_H__
