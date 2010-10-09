/*!
 * \file ModelExporterDesc.cpp
 * \date 1-3-2010 21:01:34
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelExporter.h"
#include "ModelExporterDesc.h"

CModelExporterDesc::CModelExporterDesc()
{
	// TODO: 
}

CModelExporterDesc::~CModelExporterDesc()
{
	// TODO: 
}

int CModelExporterDesc::IsPublic()
{
	return 1;
}

void* CModelExporterDesc::Create(BOOL loading /*= FALSE*/)
{
	return new CModelExporter();
}

const TCHAR* CModelExporterDesc::ClassName()
{
	return _T("CModelExporterDesc");
}

SClass_ID CModelExporterDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

Class_ID CModelExporterDesc::ClassID()
{
	return Class_ID(0x6ebf4c58, 0x796f25f4);
}

const TCHAR* CModelExporterDesc::Category()
{
	return _T("scene export");
}
