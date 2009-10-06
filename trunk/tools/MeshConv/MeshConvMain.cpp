/*!
 * \file MeshConvMain.cpp
 * \date 30-7-2009 20:14:13
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "MeshConvMain.h"
#include "Ms3dConv.h"

#include <OEOS.h>

int main(int argc, char** argv)
{
	if (argc < 2) return 0;

	tstring strFileName;
	if (!COEOS::char2tchar(strFileName, argv[1])) return 0;

	if (COEOS::Initialize())
	{
		CMs3dConv conv;
		if (conv.LoadFromFile(strFileName))
		{
			strFileName.append(t(".mesh"));
			conv.DoConvert(strFileName);
		}
	}

	COEOS::Terminate();
	return 0;
}
