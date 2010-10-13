/*!
 * \file OEBinormalRender_Impl.h
 * \date 10-12-2010 22:19:42
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OEBINORMALRENDER_IMPL_H__
#define __OEBINORMALRENDER_IMPL_H__

#include <OECore/IOERender.h>

class COEBinormalRender_Impl : public IOERender
{
public:
	typedef std::vector<VERTEX_LINE> TV_VERTEX_LINE;
	typedef std::vector<ushort> TV_INDEX;

public:
	RTTI_DEF(COEBinormalRender_Impl, IOERender);

	COEBinormalRender_Impl();
	virtual ~COEBinormalRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool Init();
	void Destroy();

	int GetBinormalOffset(int nVertDescMask);
	int GetVertexSize(int nVertDescMask);
	void CreateBinormalLine(TV_VERTEX_LINE& vLines, TV_INDEX& vIndex, IOEPiece* pPiece);

private:
	TV_VERTEX_LINE m_vLines;
	TV_INDEX m_vIndex;
	IOEShader* m_pShader;

};
#endif // __OEBINORMALRENDER_IMPL_H__
