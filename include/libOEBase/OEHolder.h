/*!
 * \file OEHolder.h
 * \date 24-5-2009 0:35:51
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEHOLDER_H__
#define __OEHOLDER_H__

#include "OEBaseType.h"
#include <map>

class COEHolder
{
public:
	typedef struct INTERFACE_INFO_tag
	{
		tstring strClassName;
		void** ppInterface;
	} INTERFACE_INFO;

	typedef std::map<tstring, INTERFACE_INFO> TM_INTERFACE_INFO;

public:
	COEHolder();
	~COEHolder();

	void AddInterface(const tstring& strClassName, void** ppInterface);
	void MergeFrom(const COEHolder& Holder);

private:
	void Init();
	void Destroy();

private:
	TM_INTERFACE_INFO m_InterfaceInfoMap;

};

#endif // __OEHOLDER_H__
