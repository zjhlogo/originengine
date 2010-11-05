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
		TS("Usage: ImageGenerator.exe -command parameters\n")
		TS("    -c compound image ([in]RGB image) ([in]Alpha image) ([out]RGBA image)\n")
		TS("    -x extract image ([in]RGBA image) ([out]Alpha image) ([out]RGB image)\n")
		TS("example: ImageGenerator.exe -c output.png diffuse.png alpha.png\n");

	std::string strUsage;
	COEOS::tchar2char(strUsage, s_szUsage);
	std::cout << strUsage << std::endl;
}

bool MergeImage(const tstring& strFileRGB, const tstring& strFileAlpha, const tstring& strFileRGBA)
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

	BOOL bOK = FreeImage_Save(FIF_PNG, dibRGB32, strFileRGBA.c_str());
	FreeImage_Unload(dibRGB32);
	FreeImage_Unload(dibAlpha8);
	return (bOK == TRUE);
}

bool ExtractImage(const tstring& strFileIn, const tstring& strFileOutRGB, const tstring& strFileOutAlpha)
{
	FREE_IMAGE_FORMAT fifBitmap = FreeImage_GetFileType(strFileIn.c_str());
	FIBITMAP* dibBitmap32 = FreeImage_Load(fifBitmap, strFileIn.c_str(), 0);
	FIBITMAP* dibBitmap24 = FreeImage_ConvertTo24Bits(dibBitmap32);
	FIBITMAP* dibAlpha = FreeImage_GetChannel(dibBitmap32, FICC_ALPHA);

	FreeImage_Save(FIF_PNG, dibBitmap24, strFileOutRGB.c_str());
	FreeImage_Save(FIF_PNG, dibAlpha, strFileOutAlpha.c_str());

	FreeImage_Unload(dibAlpha);
	FreeImage_Unload(dibBitmap24);
	FreeImage_Unload(dibBitmap32);
	return true;
}

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		PrintUsage();
		return 0;
	}

	tstring strCommand;
	COEOS::char2tchar(strCommand, argv[1]);
	if (strCommand == TS("-c"))
	{
		tstring strRGB;
		tstring strAlpha;
		tstring strRGBA;

		COEOS::char2tchar(strRGB, argv[2]);
		COEOS::char2tchar(strAlpha, argv[3]);
		COEOS::char2tchar(strRGBA, argv[4]);

		bool bOK = MergeImage(strRGB, strAlpha, strRGBA);
	}
	else if (strCommand == TS("-x"))
	{
		tstring strRGBA;
		tstring strRGB;
		tstring strAlpha;

		COEOS::char2tchar(strRGBA, argv[2]);
		COEOS::char2tchar(strRGB, argv[3]);
		COEOS::char2tchar(strAlpha, argv[4]);

		bool bOK = ExtractImage(strRGBA, strRGB, strAlpha);
	}

	return 0;
}
