/*!
 * \file IOEResMgr.h
 * \date 13-2-2010 9:27:35
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERESMGR_H__
#define __IOERESMGR_H__

#include "IOEObject.h"
#include "IOEModel.h"
#include "IOEMesh.h"
#include "IOEBones.h"
#include "IOEControl.h"
#include "IOERender.h"
#include "OEBaseTypeEx.h"
#include "IOEXmlNode.h"

class IOEResMgr : public IOEObject
{
public:
	IOEResMgr() {};
	virtual ~IOEResMgr() {};

	virtual IOEModel* CreateModel(const tstring& strFile) = 0;
	virtual IOEMesh* CreateMesh(const tstring& strFile) = 0;
	virtual IOEBones* CreateBones(const tstring& strFile) = 0;

	virtual bool CreateMaterial(MATERIAL& MaterialOut, IOEXmlNode* pXmlMaterial) = 0;
	virtual void DestroyMaterial(MATERIAL& Material) = 0;

	virtual void SetDefaultDir(const tstring& strDir) = 0;
	virtual const tstring& GetDefaultDir() = 0;
};

extern IOEResMgr* g_pOEResMgr;

#endif // __IOERESMGR_H__
