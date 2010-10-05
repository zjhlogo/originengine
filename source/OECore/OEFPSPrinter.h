/*!
 * \file OEFPSPrinter.h
 * \date 10-4-2010 23:32:07
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEFPSPRINTER_H__
#define __OEFPSPRINTER_H__

#include <OECore/IOERenderableObject.h>
#include <OEUI/IOEUIFontMgr.h>
#include <OEUI/IOEUIStringMgr.h>

class COEFPSPrinter : public IOERenderableObject
{
public:
	COEFPSPrinter();
	virtual ~COEFPSPrinter();

	virtual void Update(float fDetailTime);
	virtual void Render(float fDetailTime);

private:
	void CalculateFPS();

private:
	float m_fCurrFPS;
	float m_fLastFPSTime;
	int m_nFPSCount;
	IOEUIFont* m_pFontFPS;
	IOEUIString* m_pStringFPS;

};
#endif // __OEFPSPRINTER_H__
