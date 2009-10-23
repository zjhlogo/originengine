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

#include <IOEMesh.h>

#include <vector>
#include <map>

class COEModel_Impl : public IOEModel
{
public:
	typedef std::vector<CMatrix4x4> VMATRIX;

public:
	COEModel_Impl(const tstring& strFileName);
	virtual ~COEModel_Impl();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

	virtual IOEMesh* GetMesh();
	virtual int GetNumMatrixPalette();
	virtual CMatrix4x4* GetMatrixPalette();

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);

private:
	IOEMesh* m_pMesh;
	VMATRIX m_vmatSkin;
	float m_fTotalTime;

};

#endif // __OEMODEL_IMPL_H__
