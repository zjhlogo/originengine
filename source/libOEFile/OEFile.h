/********************************************************************
	created:	2009/05/31
	filename: 	originengine\source\libOEFile\OEFile.h
	author:		daishuidi
	
	purpose:	�ļ���
*********************************************************************/
#ifndef __OEFILE_H__
#define __OEFILE_H__

#include "OEBasicType.h"

struct SOEFile
{
	SOEFile()
	{
		m_pFile = NULL;
		memset(pstrFileName,0,FILENAME_MAX);
	}
	FILE	*m_pFile;
	tchar pstrFileName[FILENAME_MAX];
	//other information to add
};


//�ļ��򿪣��رղ���
SOEFile*	OEfopen(const tchar *pstrFileName, const tchar *mode);
int			OEfclose(SOEFile *fp);

//�ļ���д����
int			OEfgetc(SOEFile *fp);
int			OEfputc(int ch,SOEFile *fp);
tchar*		OEfgets(tchar *str, int n, SOEFile* fp);
int			OEfputs(const tchar *str, SOEFile *fp);
//int			OEfprintf(SOEFile *fp, tchar *format, ...);
//int			OEfscanf(SOEFile *fp, tchar *format, ...);
size_t		OEfread(void *buffer, size_t size, size_t count, SOEFile *fp);
size_t		OEfwrite(const void *buffer,size_t size, size_t count, SOEFile *fp);
int			OEgetw(SOEFile *fp);
int			OEputw(int n, SOEFile *fp);

//�ļ�״̬���
int			OEfeof(SOEFile *fp);
int			OEferror(SOEFile *fp);
void		OEclearerr(SOEFile *fp);
long		OEftell(SOEFile *fp);
//�ļ���λ
void		OErewind(SOEFile *fp);
int			OEfseek(SOEFile *fp, long offset, int whence);
#endif