// splitter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum CONST_DEFILE
{
	SPLIT_Z	= 5,
	SPLIT_X = 5,

	TILE_SIZE = 65,
};

typedef unsigned short ushort;

void WriteRaw(int x, ushort* pField)
{
	char szFileName[128] = {'\0'};
	sprintf(szFileName, "map_%d.raw", x);
	FILE* pFile = _tfopen(szFileName, _T("wba+"));

	if (!pFile)
		return;

	fwrite(pField, sizeof(ushort), TILE_SIZE*TILE_SIZE, pFile);
	fclose(pFile);
}

void GetMapByIndex(int nIndex)
{
	FILE* pFile = _tfopen("maptile.raw", _T("rb")); 

	if (!pFile)
		return;

	long nPos   = 0;
	long nLen   = 0;
	int  nCount = -1;

	for (int z = 0; z < SPLIT_Z; z++)
	{
		nLen = TILE_SIZE*TILE_SIZE*sizeof(ushort)*SPLIT_X * z;

		for (int x = 0; x < SPLIT_X; x++)
		{
			nCount++;

			if (nCount != nIndex)
				continue;

			ushort* pHeightField = new ushort[TILE_SIZE*TILE_SIZE];
			memset(pHeightField, 0, sizeof(ushort) * TILE_SIZE*TILE_SIZE);

			nPos = sizeof(ushort) * x * TILE_SIZE + nLen; 
			fseek(pFile, nPos, SEEK_SET);

			for (int i = 0; i < TILE_SIZE; i++)
			{
				fread(&pHeightField[i * TILE_SIZE], sizeof(ushort), TILE_SIZE, pFile);

				nPos  += (sizeof(ushort) * (TILE_SIZE)) * SPLIT_X;
				fseek(pFile, nPos, SEEK_SET);
			}

			WriteRaw(100, pHeightField);

			fclose(pFile);
			return;
		}
	}

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
	for (int z = 0; z < SPLIT_Z; z++)
	{
		nLen = TILE_SIZE*TILE_SIZE*sizeof(ushort)*SPLIT_X * z;

		for (int x = 0; x < SPLIT_X; x++)
		{
			ushort* pHeightField = new ushort[TILE_SIZE*TILE_SIZE];
			memset(pHeightField, 0, sizeof(ushort) * TILE_SIZE*TILE_SIZE);

			nPos = sizeof(ushort) * x * TILE_SIZE + nLen; 
			fseek(pFile, nPos, SEEK_SET);

			for (int i = 0; i < TILE_SIZE; i++)
			{
				fread(&pHeightField[i * TILE_SIZE], sizeof(ushort), TILE_SIZE, pFile);

				nPos  += (sizeof(ushort) * (TILE_SIZE)) * SPLIT_X;
				fseek(pFile, nPos, SEEK_SET);
			}

			WriteRaw(nCount, pHeightField);

			nCount++;
		}
	}

	fclose(pFile);

	return 0;
}





