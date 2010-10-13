/*!
 * \file OETangentRender_Impl.h
 * \date 10-12-2010 22:23:03
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#ifndef __OETANGENTRENDER_IMPL_H__
#define __OETANGENTRENDER_IMPL_H__

#include <OECore/IOERender.h>

class COETangentRender_Impl : public IOERender
{
public:
	typedef std::vector<VERTEX_LINE> TV_VERTEX_LINE;
	typedef std::vector<ushort> TV_INDEX;

public:
	RTTI_DEF(COETangentRender_Impl, IOERender);

	COETangentRender_Impl();
	virtual ~COETangentRender_Impl();

	virtual bool Render(IOERenderData* pRenderData);

private:
	bool Init();
	void Destroy();

	int GetTangentOffset(int nVertDescMask);
	int GetVertexSize(int nVertDescMask);
	void CreateTangentLine(TV_VERTEX_LINE& vNormalLine, TV_INDEX& vIndex, IOEPiece* pPiece);

private:
	TV_VERTEX_LINE m_vLines;
	TV_INDEX m_vIndex;
	IOEShader* m_pShader;

};
#endif // __OETANGENTRENDER_IMPL_H__
