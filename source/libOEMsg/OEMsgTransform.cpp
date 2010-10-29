/*!
 * \file OEMsgTransform.cpp
 * \date 10-29-2010 9:32:08
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include <libOEMsg/OEMsgTransform.h>
#include <libOEMsg/OEMsgList.h>

COEMsgTransform::COEMsgTransform(uint nMsgID, const CMatrix4x4& matrix, TRANSFORM_TYPE eType)
:IOEMsg(nMsgID)
{
	SetMatrix(matrix);
	m_eType = eType;
}

COEMsgTransform::COEMsgTransform(COEDataBufferRead* pDBRead)
:IOEMsg(pDBRead)
{
	FromBuffer(pDBRead);
}

COEMsgTransform::~COEMsgTransform()
{
	// TODO: 
}

void COEMsgTransform::SetMatrix(const CMatrix4x4& matrix)
{
	m_Matrix = matrix;
}

const CMatrix4x4& COEMsgTransform::GetMatrix()
{
	return m_Matrix;
}

TRANSFORM_TYPE COEMsgTransform::GetType()
{
	return m_eType;
}

bool COEMsgTransform::FromBuffer(COEDataBufferRead* pDBRead)
{
	pDBRead->Read(&m_Matrix, sizeof(m_Matrix));
	pDBRead->Read(&m_eType, sizeof(m_eType));
	return true;
}

bool COEMsgTransform::ToBuffer(COEDataBufferWrite* pDBWrite)
{
	pDBWrite->Write(&m_Matrix, sizeof(m_Matrix));
	pDBWrite->Write(&m_eType, sizeof(m_eType));
	return true;
}
