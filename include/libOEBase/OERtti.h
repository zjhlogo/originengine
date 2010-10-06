/*!
 * \file OERtti.h
 * \date 10-5-2010 18:08:14
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OERTTI_H__
#define __OERTTI_H__

#include "OEBaseType.h"

class COERtti
{
public:
	COERtti(const tstring& strTypeName, const COERtti* pBaseRtti);
	~COERtti();

	const tstring& GetTypeName() const;
	const COERtti* GetBaseRtti() const;

	bool IsType(const tstring& strTypeName) const;
	bool IsDerived(const tstring& strTypeName) const;

private:
	tstring m_strTypeName;
	const COERtti* m_pBaseRtti;

};

class CNoRtti
{
public:
	CNoRtti() {};
	~CNoRtti() {};

	static const COERtti* __RttiData() {return NULL;};

};

#define RTTI_DEF(name, basename)												\
static const COERtti* __RttiData()												\
{																				\
	static const COERtti s_Rtti(TS(#name), basename::__RttiData());				\
	return &s_Rtti;																\
};																				\
virtual const COERtti* GetRtti()												\
{																				\
	return __RttiData();														\
}																				\

#endif // __OERTTI_H__
