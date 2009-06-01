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

class COED3DShaderMgr_Impl : public IOEShaderMgr
{
public:
	COED3DShaderMgr_Impl();
	virtual ~COED3DShaderMgr_Impl();

	virtual IOEShader* CreateShader(const IOEVertDecl::ELEMENT* pElement, const tchar* pstrFileName);

private:
	void Init();
	void Destroy();

};
#endif // __OED3DSHADERMGR_IMPL_H__
