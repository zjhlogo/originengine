/*!
 * \file IOEMaterialsList.h
 * \date 10-29-2010 19:15:52
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __IOEMATERIALSLIST_H__
#define __IOEMATERIALSLIST_H__

#include "IOEMaterial.h"

class IOEMaterialsList : public IOEObject
{
public:
	RTTI_DEF(IOEMaterialsList, IOEObject);

	IOEMaterialsList() {};
	virtual ~IOEMaterialsList() {};

	virtual IOEMaterial* GetMaterial(int nID) = 0;
	virtual int GetNumMaterial() = 0;

};
#endif // __IOEMATERIALSLIST_H__