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
	typedef std::map<tstring, IOEMesh*> TM_MESH;
	typedef std::map<tstring, IOEBones*> TM_BONES;

public:
	COEResMgr_Impl();
	virtual ~COEResMgr_Impl();

	virtual IOEModel* CreateModel(const tstring& strFile);
	virtual IOEMesh* CreateMesh(const tstring& strFile);
	virtual IOEBones* CreateBones(const tstring& strFile);

	virtual bool CreateMaterial(MATERIAL& MaterialOut, IOEXmlNode* pXmlMaterial);
	virtual void DestroyMaterial(MATERIAL& Material);

	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

private:
	bool Init();
	void Destroy();

	void EmptyMaterial(MATERIAL& MaterialOut);
	IOEShader* CreateShaderFromVertDecl(int nVertDecl, const tstring& strFile);

	bool GetMediaFilePath(tstring& strFilePathOut, const tstring& strFile);

private:
	TM_MESH m_MeshMap;
	TM_BONES m_BonesMap;
	tstring m_strDefaultDir;

};
#endif // __OERESMGR_IMPL_H__
