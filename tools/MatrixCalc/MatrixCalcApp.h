/*!
 * \file MatrixCalcApp.h
 * \date 27-2-2010 9:09:30
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MATRIXCALCAPP_H__
#define __MATRIXCALCAPP_H__

#include <wx/app.h>

class CMatrixCalcApp : public wxApp
{
public:
	CMatrixCalcApp();
	virtual ~CMatrixCalcApp();

	virtual bool OnInit();

};
#endif // __MATRIXCALCAPP_H__
