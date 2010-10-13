/*!
 * \file OENormalRender_Impl.h
 * \date 10-12-2010 20:15:18
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OENORMALRENDER_IMPL_H__
#define __OENORMALRENDER_IMPL_H__

#include <OECore/IOERender.h>

class COENormalRender_Impl : public IOERender
{
public:
	typedef std::vector<VERTEX_LINE> TV_VERTEX_LINE;
	typedef std::vector<ushort> TV_INDEX;

public:
	RTTI_DEF(COENormalRender_Impl, IOERender);

	COENormalRender_Impl();
	virtual ~COENormalRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool Init();
	void Destroy();

	int GetNormalOffset(int nVertDescMask);
	int GetVertexSize(int nVertDescMask);
	void CreateNormalLine(TV_VERTEX_LINE& vNormalLine, TV_INDEX& vIndex, IOEPiece* pPiece);

private:
	TV_VERTEX_LINE m_vLines;
	TV_INDEX m_vIndex;
	IOEShader* m_pShader;

};
#endif // __OENORMALRENDER_IMPL_H__
