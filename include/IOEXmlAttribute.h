/*!
 * \file IOEXmlAttribute.h
 * \date 11-10-2009 10:02:08
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEXMLATTRIBUTE_H__
#define __IOEXMLATTRIBUTE_H__

#include "IOEObject.h"

class IOEXmlAttribute : public IOEObject
{
public:
	IOEXmlAttribute(){};
	virtual ~IOEXmlAttribute(){};

	virtual const tstring& GetName() = 0;
	virtual void SetName(const tstring& strName) = 0;

	virtual const tstring& GetValue() = 0;
	virtual void SetValue(const tstring& strValue) = 0;

};
#endif // __IOEXMLATTRIBUTE_H__
