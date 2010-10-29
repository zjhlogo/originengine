/*!
 * \file OEMsgTransform.h
 * \date 10-29-2010 9:32:00
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGTRANSFORM_H__
#define __OEMSGTRANSFORM_H__

#include "../libOEBase/IOEMsg.h"
#include "../OECore/OERenderState.h"

class COEMsgTransform : public IOEMsg
{
public:
	COEMsgTransform(uint nMsgID, const CMatrix4x4& matrix, TRANSFORM_TYPE eType);
	COEMsgTransform(COEDataBufferRead* pDBRead);
	virtual ~COEMsgTransform();

	void SetMatrix(const CMatrix4x4& matrix);
	const CMatrix4x4& GetMatrix();
	TRANSFORM_TYPE GetType();

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	CMatrix4x4 m_Matrix;
	TRANSFORM_TYPE m_eType;

};

#endif // __OEMSGTRANSFORM_H__
