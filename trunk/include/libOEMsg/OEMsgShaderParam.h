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
#include "../OECore/IOEMaterial.h"

class COEMsgShaderParam : public IOEMsg
{
public:
	COEMsgShaderParam(IOEShader* pShader, IOEMaterial* pMaterial);
	COEMsgShaderParam(COEDataBufferRead* pDBRead);
	virtual ~COEMsgShaderParam();

	IOEShader* GetShader() const;
	IOEMaterial* GetMaterial() const;

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	IOEShader* m_pShader;
	IOEMaterial* m_pMaterial;

};
#endif // __OEMSGSHADERPARAM_H__
