/*!
 * \file OEMsgObjectAttach.h
 * \date 10-24-2010 8:29:56
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGOBJECTATTACH_H__
#define __OEMSGOBJECTATTACH_H__

#include "../libOEBase/IOEMsg.h"
#include "../libOEBase/IOEObject.h"
#include "../OECore/IOENode.h"

class COEMsgObjectAttach : public IOEMsg
{
public:
	COEMsgObjectAttach(IOENode* pNode);
	COEMsgObjectAttach(COEDataBufferRead* pDBRead);
	virtual ~COEMsgObjectAttach();

	IOENode* GetNode();

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	IOENode* m_pNode;


};
#endif // __OEMSGOBJECTATTACH_H__
