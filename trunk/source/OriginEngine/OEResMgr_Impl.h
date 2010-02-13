/*!
 * \file OEResMgr_Impl.h
 * \date 13-2-2010 9:41:01
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OERESMGR_IMPL_H__
#define __OERESMGR_IMPL_H__

#include <IOEResMgr.h>
#include <map>

class COEResMgr_Impl : public IOEResMgr
{
public:
	typedef std::map<tstring, IOEMesh*> MESH_MAP;

public:
	COEResMgr_Impl();
	virtual ~COEResMgr_Impl();

	virtual IOEModel* CreateModel(const tstring& strFile);
	virtual IOEMesh* CreateMesh(const tstring& strFile);

	virtual bool CreatePieces(TV_PIECE& vPiecesOut, const tstring& strFile);
	virtual void DestroyPieces(TV_PIECE& vPieces);

	virtual bool CreateBones(TV_BONE& vBonesOut, const tstring& strFile);
	virtual void DestroyBones(TV_BONE& vBones);

private:
	void Init();
	void Destroy();

private:
	MESH_MAP m_MeshMap;

};
#endif // __OERESMGR_IMPL_H__
