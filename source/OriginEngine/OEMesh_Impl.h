/*!
 * \file OEMesh_Impl.h
 * \date 30-7-2009 16:50:22
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMESH_IMPL_H__
#define __OEMESH_IMPL_H__

#include <IOEMesh.h>

class COEMesh_Impl : public IOEMesh
{
public:
	COEMesh_Impl(const tstring& strFileName);
	virtual ~COEMesh_Impl();

	virtual void Render(float fDetailTime);

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);

private:

};
#endif // __OEMESH_IMPL_H__
