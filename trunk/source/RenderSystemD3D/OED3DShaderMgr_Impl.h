/*!
 * \file OED3DShaderMgr_Impl.h
 * \date 1-6-2009 16:37:59
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DSHADERMGR_IMPL_H__
#define __OED3DSHADERMGR_IMPL_H__

#include <OECore/IOEShaderMgr.h>
#include <map>

class COED3DShaderMgr_Impl : public IOEShaderMgr
{
public:
	typedef std::map<tstring, IOEShader*> SHADER_MAP;

public:
	COED3DShaderMgr_Impl();
	virtual ~COED3DShaderMgr_Impl();

	virtual bool Initialize();
	virtual void Terminate();

	virtual IOEShader* CreateShader(const VERT_DECL_ELEMENT* pElement, const tstring& strFile);
	virtual IOEShader* CreateDefaultShader(DEFAULT_SHADER_TYPE eType);

	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

private:
	bool Init();
	void Destroy();

	IOEShader* FindShader(const tstring& strFilePath);

	const VERT_DECL_ELEMENT* GetDefaultVertDecl(DEFAULT_SHADER_TYPE eType);
	bool GetDefaultShaderFile(tstring& strOut, DEFAULT_SHADER_TYPE eType);
	IOEShader* FindDefaultShader(DEFAULT_SHADER_TYPE eType);
	bool GetShaderFilePath(tstring& strFilePathOut, const tstring& strFile);

private:
	SHADER_MAP m_ShaderMap;
	IOEShader* m_pDefaultShader[DST_NUM];
	tstring m_strDefaultDir;

};
#endif // __OED3DSHADERMGR_IMPL_H__
