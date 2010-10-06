/*!
 * \file IOEVertDecl.h
 * \date 24-5-2009 17:48:28
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOEVERTDECL_H__
#define __IOEVERTDECL_H__

#include "OEBaseTypeEx.h"
#include "../libOEBase/IOEObject.h"

class IOEVertDecl : public IOEObject
{
public:
	RTTI_DEF(IOEVertDecl, IOEObject);

	IOEVertDecl() {};
	virtual ~IOEVertDecl() {};

	virtual int GetStrideSize() const = 0;

};

#endif // __IOEVERTDECL_H__
