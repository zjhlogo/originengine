/*!
 * \file BaseConv.h
 * \date 30-7-2009 20:23:10
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __BASECONV_H__
#define __BASECONV_H__

#include <OEBasicType.h>

class CBaseConv
{
public:
	CBaseConv();
	virtual ~CBaseConv();

	virtual bool LoadFromFile(const tstring& strFileName) = 0;
	virtual bool DoConvert(const tstring& strFileName) = 0;

};
#endif // __BASECONV_H__
