/*!
 * \file OEAnimData_Impl.h
 * \date 10-29-2010 18:49:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEANIMDATA_IMPL_H__
#define __OEANIMDATA_IMPL_H__

#include <OECore/IOEAnimData.h>

class COEAnimData_Impl : public IOEAnimData
{
public:
	RTTI_DEF(COEAnimData_Impl, IOEAnimData);

	COEAnimData_Impl(const tstring& strFile);
	virtual ~COEAnimData_Impl();

	virtual IOESkeleton* GetSkeleton();
	virtual float GetAnimLength();

	virtual void SetCurrTime(float fCurrTime);
	virtual float GetCurrTime();

	virtual CMatrix4x4* GetSkinMatrixs();
	virtual int GetNumSkinMatrixs();

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFile);

private:
	IOESkeleton* m_pSkeleton;
	float m_fAnimLength;
	float m_fCurrTime;
	CMatrix4x4* m_pSkinMatrixs;
	int m_nNumSkinMatrixs;

};
#endif // __OEANIMDATA_IMPL_H__
