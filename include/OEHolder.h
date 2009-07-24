/*!
 * \file OEHolder.h
 * \date 24-5-2009 0:35:51
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEHOLDER_H__
#define __OEHOLDER_H__

#include "OEBasicType.h"
#include <map>

class COEHolder
{
public:
	typedef struct INTERFACE_INFO_tag
	{
		tstring strClassName;
		void** ppInterface;
	} INTERFACE_INFO;

	typedef std::map<tstring, INTERFACE_INFO> INTERFACE_INFO_MAP;

public:
	COEHolder();
	~COEHolder();

	void SetupInterfaces();

	bool Initialize();
	void Terminate();

	void AddInterface(const tstring& strClassName, void** ppInterface);
	void MergeInterface(const COEHolder& Holder);

private:
	INTERFACE_INFO_MAP m_InfoMap;

};

extern COEHolder g_OEHolder;

#endif // __OEHOLDER_H__
