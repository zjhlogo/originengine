/*!
 * \file IOEResMgr.h
 * \date 13-2-2010 9:27:35
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERESMGR_H__
#define __IOERESMGR_H__

#include "IOEMgr.h"
#include "IOEModel.h"
#include "IOEMesh.h"
#include "IOESkeleton.h"
#include "IOEMaterial.h"
#include "IOEXmlNode.h"

class IOEResMgr : public IOEMgr
{
public:
	IOEResMgr() {};
	virtual ~IOEResMgr() {};

	virtual IOEModel* CreateModel(const tstring& strFile) = 0;
	virtual IOEMesh* CreateMesh(const tstring& strFile) = 0;
	virtual IOESkeleton* CreateSkeleton(const tstring& strFile) = 0;
	virtual IOEMaterial* CreateMaterial(IOEXmlNode* pXmlMaterial) = 0;

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;
};

extern IOEResMgr* g_pOEResMgr;

#endif // __IOERESMGR_H__
