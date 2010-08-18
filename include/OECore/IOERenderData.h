/*!
 * \file IOERenderData.h
 * \date 12-2-2010 20:00:23
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __IOERENDERDATA_H__
#define __IOERENDERDATA_H__

#include "../libOEBase/IOEObject.h"
#include "IOEMesh.h"

class IOERenderData : public IOEObject
{
public:
	IOERenderData(uint nType) {m_nType = nType;};
	virtual ~IOERenderData() {};

	uint GetType() {return m_nType;};

	virtual IOEMesh* GetMesh() = 0;

private:
	uint m_nType;

};
#endif // __IOERENDERDATA_H__
