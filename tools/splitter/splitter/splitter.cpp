// splitter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum CONST_DEFILE
{
	SPLITTER_COUNT	= 5,
	TILE_SIZE		= 65,
	TILE_WIDTH		= 400,
	TILE_HEIGHT		= 400
};

typedef unsigned short uint;

void WriteRaw(int x, uint* pField)
{
	char szFileName[128] = {'\0'};
	sprintf(szFileName, "map_%d.raw", x);
	FILE* pFile = _tfopen(szFileName, _T("wba+"));

	if (!pFile)
		return;

	fwrite(pField, sizeof(uint), TILE_SIZE*TILE_SIZE, pFile);
	fclose(pFile);
}



int _tmain(int argc, _TCHAR* argv[])
{
	FILE* pFile = _tfopen("maptile.raw", _T("rb")); 

	if (!pFile)
		return 0;

	long nPos = 0;
	long nLen = 0;
	nPos = ftell(pFile);

	int nCount = 0;
	for (int y = 0; y < SPLITTER_COUNT; y++)
	{
		nLen = ftell(pFile);

		for (int x = 0; x < SPLITTER_COUNT; x++)
		{
			uint* pHeightField = new uint[TILE_SIZE*TILE_SIZE];
			memset(pHeightField, 0, sizeof(uint) * TILE_SIZE*TILE_SIZE);

			nPos = sizeof(uint) * x * TILE_SIZE + nLen; 
			fseek(pFile, nPos, SEEK_SET);

			for (int i = 0; i < TILE_SIZE; i++)
			{
				fread(&pHeightField[i * TILE_SIZE], sizeof(uint), TILE_SIZE, pFile);

				nPos  += (sizeof(uint) * (TILE_SIZE)) * SPLITTER_COUNT;
				fseek(pFile, nPos, SEEK_SET);
			}

			WriteRaw(nCount, pHeightField);

			nCount++;
		}
	}

	fclose(pFile);

	return 0;
}





