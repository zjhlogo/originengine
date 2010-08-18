/*!
 * \file OEMsgMouse.h
 * \date 8-17-2010 19:30:28
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEMSGMOUSE_H__
#define __OEMSGMOUSE_H__

#include "IOEMsg.h"

class COEMsgMouse : public IOEMsg
{
public:
	COEMsgMouse(uint nMsgID);
	COEMsgMouse(COEDataBufferRead* pDBRead);
	virtual ~COEMsgMouse();

	void SetPos(int nX, int nY);
	int GetPosX() const;
	int GetPosY() const;

	void SetWheel(int nWheel);
	int GetWheel() const;

protected:
	virtual bool FromBuffer(COEDataBufferRead* pDBRead);
	virtual bool ToBuffer(COEDataBufferWrite* pDBWrite);

private:
	int m_nX;
	int m_nY;
	int m_nWheel;

};
#endif // __OEMSGMOUSE_H__
