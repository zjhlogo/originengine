/*!
 * \file OED3DShaderMgr_Impl.h
 * \date 1-6-2009 16:37:59
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DSHADERMGR_IMPL_H__
#define __OED3DSHADERMGR_IMPL_H__

#include <IOEShaderMgr.h>
#include <map>

class COED3DShaderMgr_Impl : public IOEShaderMgr
{
public:
	typedef std::map<tstring, IOEShader*> SHADER_MAP;

public:
	COED3DShaderMgr_Impl();
	virtual ~COED3DShaderMgr_Impl();

	virtual IOEShader* CreateShader(const VERT_DECL_ELEMENT* pElement, const tstring& strFile);
	virtual IOEShader* CreateDefaultShader(DEFAULT_SHADER_TYPE eType);

private:
	bool Init();
	void Destroy();

	IOEShader* FindShader(const tstring& strLowerFile);

	const VERT_DECL_ELEMENT* GetDefaultVertDecl(DEFAULT_SHADER_TYPE eType);
	bool GetDefaultShaderFile(tstring& strOut, DEFAULT_SHADER_TYPE eType);
	IOEShader* FindDefaultShader(DEFAULT_SHADER_TYPE eType);

private:
	SHADER_MAP m_ShaderMap;
	IOEShader* m_pDefaultShader[DST_NUM];

};
#endif // __OED3DSHADERMGR_IMPL_H__
