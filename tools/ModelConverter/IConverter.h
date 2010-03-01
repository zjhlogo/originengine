/*!
 * \file IConverter.h
 * \date 1-3-2010 21:19:05
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __ICONVERTER_H__
#define __ICONVERTER_H__

#include <OEBaseType.h>

class IConverter
{
public:
	IConverter();
	virtual ~IConverter();

	virtual bool CanConvert(const tstring& strFile) = 0;
	virtual bool DoConvert(const tstring& strFileIn, const tstring& strFileOut) = 0;

};
#endif // __ICONVERTER_H__
