/*!
 * \file IBaseConv.h
 * \date 10-20-2009 15:22:26
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IBASECONV_H__
#define __IBASECONV_H__

#include <OEBaseType.h>

class IBaseConv
{
public:
	IBaseConv();
	virtual ~IBaseConv();

	virtual bool CanConvert(const tstring& strFileExt) = 0;
	virtual bool DoConvert(const tstring& strFileIn, const tstring& strFileOut) = 0;

};
#endif // __IBASECONV_H__
