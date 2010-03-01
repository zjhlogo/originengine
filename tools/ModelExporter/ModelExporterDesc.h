/*!
 * \file ModelExporterDesc.h
 * \date 1-3-2010 21:01:27
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MODELEXPORTERDESC_H__
#define __MODELEXPORTERDESC_H__

#include <max.h>

class CModelExporterDesc : public ClassDesc
{
public:
	CModelExporterDesc();
	virtual ~CModelExporterDesc();

	virtual int IsPublic();							// Show this in create branch?
	virtual void* Create(BOOL loading = FALSE);		// return a pointer to an instance of the class.

	virtual const TCHAR* ClassName();
	virtual SClass_ID SuperClassID();
	virtual Class_ID ClassID();
	virtual const TCHAR* Category();				// primitive/spline/loft/ etc

};
#endif // __MODELEXPORTERDESC_H__
