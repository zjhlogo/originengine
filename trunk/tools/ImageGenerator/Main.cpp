/*!
 * \file Main.cpp
 * \date 8-19-2010 18:51:31
 * 
 * 
 * \author zjhlogo (zjhlogo@gmail.com)
 */
#include "Main.h"
#include <libOEBase/OEBaseType.h>
#include <libOEBase/OEOS.h>
#include <FreeImage.h>
#include <iostream>
#include <string>

#ifdef _UNICODE
#define FreeImage_GetFileType FreeImage_GetFileTypeU
#define FreeImage_Load FreeImage_LoadU
#define FreeImage_Save FreeImage_SaveU
#endif // _UNICODE

void PrintUsage()
{
	static const tchar* s_szUsage = 
		TS("Image Generator version 1.0\n")
		TS("author: zjhlogo, zjhlogo@gmail.com\n")
		TS("Usage: ImageGenerator.exe (output image) (RGB image) (Alpha image)\n")
		TS("example: ImageGenerator.exe output.png diffuse.png alpha.png\n");

	std::string strUsage;
	COEOS::tchar2char(strUsage, s_szUsage);
	std::cout << strUsage << std::endl;
}

bool MergeImage(const tstring& strFileOut, const tstring& strFileRGB, const tstring& strFileAlpha)
{
	FREE_IMAGE_FORMAT fifRGB = FreeImage_GetFileType(strFileRGB.c_str());
	FIBITMAP* dibRGB = FreeImage_Load(fifRGB, strFileRGB.c_str(), 0);
	uint nWidthRGB = FreeImage_GetWidth(dibRGB);
	uint nHeightRGB = FreeImage_GetWidth(dibRGB);

	FREE_IMAGE_FORMAT fifAlpha = FreeImage_GetFileType(strFileAlpha.c_str());
	FIBITMAP* dibAlpha = FreeImage_Load(fifAlpha, strFileAlpha.c_str(), 0);
	uint nWidthAlpha = FreeImage_GetWidth(dibAlpha);
	uint nHeightAlpha = FreeImage_GetWidth(dibAlpha);

	if (nWidthRGB != nWidthAlpha || nHeightRGB != nHeightAlpha)
	{
		FreeImage_Unload(dibRGB);
		FreeImage_Unload(dibAlpha);
		return false;
	}

	FIBITMAP* dibRGB32 = FreeImage_ConvertTo32Bits(dibRGB);
	FIBITMAP* dibAlpha8 = FreeImage_ConvertToGreyscale(dibAlpha);
	FreeImage_Unload(dibRGB);
	FreeImage_Unload(dibAlpha);

	if (FreeImage_SetChannel(dibRGB32, dibAlpha8, FICC_ALPHA) == FALSE)
	{
		FreeImage_Unload(dibRGB32);
		FreeImage_Unload(dibAlpha8);
		return false;
	}

	BOOL bOK = FreeImage_Save(FIF_PNG, dibRGB32, strFileOut.c_str());
	FreeImage_Unload(dibRGB32);
	FreeImage_Unload(dibAlpha8);
	return (bOK == TRUE);
}


int main(int argc, char** argv)
{
	if (argc < 4)
	{
		PrintUsage();
		return 0;
	}

	tstring strOutput;
	tstring strRGB;
	tstring strAlpha;

	COEOS::char2tchar(strOutput, argv[1]);
	COEOS::char2tchar(strRGB, argv[2]);
	COEOS::char2tchar(strAlpha, argv[3]);

	bool bOK = MergeImage(strOutput, strRGB, strAlpha);
	return 0;
}
