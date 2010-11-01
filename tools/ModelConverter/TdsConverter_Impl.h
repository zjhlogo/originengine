/*!
 * \file TdsConverter_Impl.h
 * \date 10-26-2010 21:25:27
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __TDSCONVERTER_IMPL_H__
#define __TDSCONVERTER_IMPL_H__

#include "IConverter.h"
#include "format/FmtTds.h"

#include <OECore/OEFmtMesh.h>
#include <vector>

class CTdsConverter_Impl : public IConverter
{
public:
	typedef std::vector<ushort> TV_USHORT;
	typedef struct INDEX_tag
	{
		ushort v1, v2, v3;
	} INDEX;

	typedef struct FILE_VERTEX_tag
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
	} FILE_VERTEX;

public:
	CTdsConverter_Impl();
	virtual ~CTdsConverter_Impl();

	virtual bool CanConvert(const tstring& strFile);
	virtual bool DoConvert(const tstring& strFileIn, const tstring& strFileOut);

private:
	void Init();
	void Destroy();

	bool LoadFromFile(const tstring& strFile);
	void ClearData();
	bool SaveToMeshFile(const tstring& strFile);
	bool SaveToXmlFile(const tstring& strFile, const tstring& strMeshFile);
	ushort RebuildIndis(TV_USHORT& IndisOut, const TSGMesh& Mesh);

private:
	TSGHeader m_TdsHeader;
	TSGMaterial* m_pMaterials;
	TSGMesh* m_pMeshes;
	TSGVertex* m_pVertices;
	uint m_nVertDecl;

	CVector3 m_vBoundingBoxMin;
	CVector3 m_vBoundingBoxMax;

};
#endif // __TDSCONVERTER_IMPL_H__
