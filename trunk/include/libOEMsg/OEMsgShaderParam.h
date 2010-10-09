/*!
 * \file OEMsgShaderParam.h
 * \date 8-17-2010 21:28:36
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGSHADERPARAM_H__
#define __OEMSGSHADERPARAM_H__

#include "../libOEBase/IOEMsg.h"
#include "../OECore/IOEShader.h"

class COEMsgShaderParam : public IOEMsg
{
public:
	COEMsgShaderParam(IOEShader* pShader);
	COEMsgShaderParam(COEDataBufferRead* pDBRead);
	virtual ~COEMsgShaderParam();

	IOEShader* GetShader() const;

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	IOEShader* m_pShader;

};
#endif // __OEMSGSHADERPARAM_H__
