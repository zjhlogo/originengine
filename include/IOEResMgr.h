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
#include "IOEControl.h"
#include "IOERender.h"
#include "OEBaseTypeEx.h"

class IOEResMgr : public IOEObject
{
public:
	IOEResMgr() {};
	virtual ~IOEResMgr() {};

	virtual IOEModel* CreateModel(const tstring& strFile) = 0;
	virtual IOEMesh* CreateMesh(const tstring& strFile) = 0;

	virtual bool CreatePieces(TV_PIECE& vPiecesOut, const tstring& strFile) = 0;
	virtual void DestroyPieces(TV_PIECE& vPieces) = 0;

	virtual bool CreateBones(TV_BONE& vBonesOut, const tstring& strFile) = 0;
	virtual void DestroyBones(TV_BONE& vBones) = 0;

};

extern IOEResMgr* g_pOEResMgr;

#endif // __IOERESMGR_H__
