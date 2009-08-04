/*!
 * \file MeshExporterDesc.h
 * \date 1-8-2009 10:27:46
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __MESHEXPORTERDESC_H__
#define __MESHEXPORTERDESC_H__

#include <max.h>

class CMeshExporterDesc : public ClassDesc
{
public:
	CMeshExporterDesc();
	virtual ~CMeshExporterDesc();

	virtual int IsPublic();							// Show this in create branch?
	virtual void* Create(BOOL loading = FALSE);		// return a pointer to an instance of the class.

	virtual const TCHAR* ClassName();
	virtual SClass_ID SuperClassID();
	virtual Class_ID ClassID();
	virtual const TCHAR* Category();				// primitive/spline/loft/ etc

};
#endif // __MESHEXPORTERDESC_H__
