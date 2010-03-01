/*!
 * \file ModelConverterMain.cpp
 * \date 1-3-2010 21:22:19
 * 
 * 
 * \author zjhlogo (zjhlogo@163.com)
 */
#include "ModelConverterMain.h"
#include "ConverterMgr.h"

#include <OEOS.h>
#include <iostream>

bool CheckParameter(int argc, char** argv)
{
	if (argc < 2)
	{
		std::cout << "usage: ModelConverter.exe filename" << std::endl;
		return false;
	}

	return true;
}

int main(int argc, char** argv)
{
	if (!COEOS::Initialize(COEOS::IS_FILE)) return 0;

	if (!CheckParameter(argc, argv))
	{
		COEOS::Terminate();
		return 0;
	}

	tstring strFileIn;
	COEOS::char2tchar(strFileIn, argv[1]);

	tstring strFileOut;
	COEOS::GetFileName(strFileOut, strFileIn);

	CConverterMgr::Get().DoConvert(strFileIn, strFileOut);

	COEOS::Terminate();
	return 0;
}