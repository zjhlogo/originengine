/*!
 * \file OEMsgObjectDestroy.h
 * \date 10-24-2010 8:29:33
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGOBJECTDESTROY_H__
#define __OEMSGOBJECTDESTROY_H__

#include "../libOEBase/IOEMsg.h"
#include "../libOEBase/IOEObject.h"

class COEMsgObjectDestroy: public IOEMsg
{
public:
	COEMsgObjectDestroy(IOEObject* pObject);
	COEMsgObjectDestroy(COEDataBufferRead* pDBRead);
	virtual ~COEMsgObjectDestroy();

	IOEObject* GetObjectHandle();

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	IOEObject* m_pObject;

};
#endif // __OEMSGOBJECTDESTROY_H__
