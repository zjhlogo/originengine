/*!
 * \file OEMsgDestroy.h
 * \date 10-8-2010 23:08:22
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGDESTROY_H__
#define __OEMSGDESTROY_H__

#include "../libOEBase/IOEMsg.h"
#include "../libOEBase/IOEObject.h"

class COEMsgDestroy: public IOEMsg
{
public:
	COEMsgDestroy(IOEObject* pObject);
	COEMsgDestroy(COEDataBufferRead* pDBRead);
	virtual ~COEMsgDestroy();

	IOEObject* GetObjectHandle();

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	IOEObject* m_pObject;

};

#endif // __OEMSGDESTROY_H__
