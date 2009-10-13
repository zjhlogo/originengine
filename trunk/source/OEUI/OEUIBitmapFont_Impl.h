/*!
 * \file OEUIBitmapFont_Impl.h
 * \date 27-7-2009 19:37:54
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OEUIBITMAPFONT_IMPL_H__
#define __OEUIBITMAPFONT_IMPL_H__

#include <OEUI/IOEUIFont.h>

#include <IOEXmlDocument.h>
#include <IOEXmlNode.h>

#include <vector>
#include <map>

class COEUIBitmapFont_Impl : public IOEUIFont
{
public:
	typedef std::map<int, CHAR_INFO> CHAR_INFO_MAP;
	typedef std::map<int, float> KERNING_INFO_MAP;
	typedef std::vector<IOETexture*> VOETEXTURE;

public:
	COEUIBitmapFont_Impl(const tstring& strFileName);
	virtual ~COEUIBitmapFont_Impl();

	virtual float GetLineHeight() const;
	virtual const CHAR_INFO* GetCharInfo(int nID) const;
	virtual float GetKerning(int nFirstID, int nSecondID) const;

private:
	void Init();
	void Destroy();

	bool Create(const tstring& strFileName);

	bool ParseCommonInfo(IOEXmlNode* pCommonNode);

	bool CreateTextures(IOEXmlNode* pPagesNode);
	void DestroyTextures();

	bool CreateCharsInfo(IOEXmlNode* pCharsNode);
	void DestroyCharsInfo();

	bool CreateKerningsInfo(IOEXmlNode* pKerningsNode);
	void DestroyKerningsInfo();

	int HashKerningID(int nFirstID, int nSecondID) const;

private:
	IOEXmlDocument* m_pXmlDocument;
	float m_fLineHeight;
	int m_nPageCount;

	VOETEXTURE m_vTexture;
	CHAR_INFO_MAP m_CharInfoMap;
	KERNING_INFO_MAP m_KerningInfoMap;

};
#endif // __OEUIBITMAPFONT_IMPL_H__
