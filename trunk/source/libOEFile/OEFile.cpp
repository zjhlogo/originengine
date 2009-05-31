
#include "OEFile.h"
#include <assert.h>
SOEFile* OEfopen( const tchar *pstrFileName, const tchar *mode )
{
	assert(pstrFileName);
	assert(mode);

	SOEFile *fp = new SOEFile;
	assert(fp);

	if (fp)
	{
		fp->m_pFile = fopen(pstrFileName,mode);
	}
	return fp;
}

int OEfclose( SOEFile *fp )
{
	assert(fp);
	int nResult = EOF;
	if (fp)
	{
		nResult = fclose(fp->m_pFile);
		SAFE_DELETE(fp);
	}
	return nResult;
}

int OEfgetc( SOEFile *fp )
{
	assert(fp);

	int nResult = EOF;
	if (fp)
	{
		nResult = fgetwc(fp->m_pFile);
	}
	return nResult;
}

int OEfputc( int ch,SOEFile *fp )
{
	assert(fp);
	int nResult = EOF;
	if (fp)
	{
		nResult = fputc(ch, fp->m_pFile);
	}
	return nResult;
}

tchar* OEfgets( tchar *str, int n, SOEFile* fp )
{
	assert(fp);
	tchar *pResult = NULL;
	if (fp)
	{
		pResult = fgets(str, n, fp->m_pFile);
	}
	return pResult;
}

int OEfputs(const tchar *str, SOEFile *fp )
{
	assert(str);
	assert(fp);
	int nResult = NULL;
	if (str && fp)
	{
		nResult = fputs(str, fp->m_pFile);
	}
	return nResult;
}

//int OEfprintf( SOEFile *fp, tchar *format, ... )
//{
//	assert(fp);
//	assert(format);
//	int nResult = 0;
//	if (fp && format)
//	{
//		nResult = fprintf(fp->m_pFile,format);
//	}
//	return nResult;
//}
//
//int OEfscanf( SOEFile *fp, tchar *format, ... )
//{
//
//}

size_t OEfread( void *buffer, size_t size, size_t count, SOEFile *fp )
{
	assert(fp);
	size_t nResult = 0;
	if (fp)
	{
		nResult = fread(buffer, size, count, fp->m_pFile);
	}
	return nResult;
}

size_t OEfwrite( const void *buffer,size_t size, size_t count, SOEFile *fp )
{
	assert(buffer);
	assert(fp);
	size_t nResult = 0;
	if (buffer && fp)
	{
		nResult = fwrite(buffer, size, count, fp->m_pFile);
	}
	return nResult;
}

int OEgetw( SOEFile *fp )
{
	assert(fp);
	int nResult = EOF;
	if (fp)
	{
		nResult = getw(fp->m_pFile);
	}
	return nResult;
}

int OEputw( int n, SOEFile *fp )
{
	assert(fp);
	int nResult = EOF;
	if (fp)
	{
		nResult = putw(n, fp->m_pFile);
	}
	return nResult;
}

int OEfeof( SOEFile *fp )
{
	assert(fp);
	int nResult = 0;
	if (fp)
	{
		nResult = feof(fp->m_pFile);
	}
	return nResult;
}

int OEferror( SOEFile *fp )
{
	assert(fp);
	int nResult = 0;
	if (fp)
	{
		nResult = ferror(fp->m_pFile);
	}
	return nResult;
}

void OEclearerr( SOEFile *fp )
{
	assert(fp);
	if (fp)
	{
		clearerr(fp->m_pFile);
	}
}

long OEftell( SOEFile *fp )
{
	assert(fp);
	long nResult = -1;
	if (fp)
	{
		nResult = ftell(fp->m_pFile);
	}
	return nResult;
}

void OErewind( SOEFile *fp )
{
	assert(fp);
	if (fp)
	{
		rewind(fp->m_pFile);
	}
}

int OEfseek( SOEFile *fp, long offset, int whence )
{
	assert(fp);
	int nResult = -1;
	if (fp)
	{
		nResult = fseek(fp->m_pFile,offset,whence);
	}
	return nResult;
}