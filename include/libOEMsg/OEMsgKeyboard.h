/*!
 * \file OEMsgKeyboard.h
 * \date 8-17-2010 19:47:20
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGKEYBOARD_H__
#define __OEMSGKEYBOARD_H__

#include "../libOEBase/IOEMsg.h"

class COEMsgKeyboard : public IOEMsg
{
public:
	COEMsgKeyboard(uint nMsgID);
	COEMsgKeyboard(COEDataBufferRead* pDBRead);
	virtual ~COEMsgKeyboard();

	void SetKeyCode(uint nKeyCode);
	uint GetKeyCode() const;

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	uint m_nKeyCode;

};
#endif // __OEMSGKEYBOARD_H__
