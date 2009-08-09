/*!
 * \file MeshExporterDesc.cpp
 * \date 1-8-2009 10:30:39
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshExporterDesc.h"
#include "MeshExporter.h"

CMeshExporterDesc::CMeshExporterDesc()
{
	// TODO: 
}

CMeshExporterDesc::~CMeshExporterDesc()
{
	// TODO: 
}

int CMeshExporterDesc::IsPublic()
{
	return 1;
}

void* CMeshExporterDesc::Create(BOOL loading /*= FALSE*/)
{
	return new CMeshExporter();
}

const TCHAR* CMeshExporterDesc::ClassName()
{
	return _T("CMeshExporterDesc");
}

SClass_ID CMeshExporterDesc::SuperClassID()
{
	return SCENE_EXPORT_CLASS_ID;
}

Class_ID CMeshExporterDesc::ClassID()
{
	return Class_ID(0x6ebf4c58, 0x796f25f4);
}

const TCHAR* CMeshExporterDesc::Category()
{
	return _T("scene export");
}
