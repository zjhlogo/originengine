/*!
 * \file OEModel_Impl.h
 * \date 9-8-2009 17:49:37
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEMODEL_IMPL_H__
#define __OEMODEL_IMPL_H__

#include <IOEModel.h>
#include <OEInterfaces.h>

class COEModel_Impl : public IOEModel
{
public:
	COEModel_Impl(const tstring& strFileName);
	virtual ~COEModel_Impl();

private:
	void Init();
	void Destroy();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	bool Create(const tstring& strFileName);

private:
	IOEMesh* m_pMesh;

};

#endif // __OEMODEL_IMPL_H__
