/*!
 * \file OED3DTextureMgr_Impl.h
 * \date 1-6-2009 14:57:44
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#ifndef __OED3DTEXTUREMGR_IMPL_H__
#define __OED3DTEXTUREMGR_IMPL_H__

#include <IOETextureMgr.h>
#include <map>

class COED3DTextureMgr_Impl : public IOETextureMgr
{
public:
	typedef std::map<tstring, IOETexture*> TM_TEXTURE;
	typedef std::vector<IOETexture*> TV_TEXTURE;

public:
	COED3DTextureMgr_Impl();
	virtual ~COED3DTextureMgr_Impl();

	virtual IOETexture* CreateTextureFromFile(const tstring& strFile);
	virtual IOETexture* CreateTexture(int nWidth, int nHeight, TEXTURE_FORMAT eFormat);

	virtual void SetDefaultDir(const tstring& strDir);
	virtual const tstring& GetDefaultDir();

private:
	bool Init();
	void Destroy();

	bool GetTextureFilePath(tstring& strFilePathOut, const tstring& strFile);

private:
	TM_TEXTURE m_FileTextureMap;
	TV_TEXTURE m_vMemoryTexture;
	tstring m_strDefaultDir;

};

#endif // __OED3DTEXTUREMGR_IMPL_H__
