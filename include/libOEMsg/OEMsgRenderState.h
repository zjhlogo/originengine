/*!
 * \file OEMsgRenderState.h
 * \date 10-29-2010 0:02:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGRENDERSTATE_H__
#define __OEMSGRENDERSTATE_H__

#include "../libOEBase/IOEMsg.h"
#include "../OECore/OERenderState.h"

class COEMsgRenderState: public IOEMsg
{
public:
	COEMsgRenderState(const COERenderState& RenderState, const tstring& strCommon = EMPTY_STRING);
	COEMsgRenderState(COEDataBufferRead* pDBRead);
	virtual ~COEMsgRenderState();

	COERenderState& GetRenderState();
	const tstring& GetCommon();

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	COERenderState m_RenderState;
	tstring m_strCommon;

};

#endif // __OEMSGRENDERSTATE_H__
