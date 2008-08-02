//! @file	Cat_StreamFile.c
// ファイルストリーム関連

#include "Cat_StreamFile.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>	// for memalign

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! 内部データ
typedef struct {
	FILE*	pFile;			/*!< ファイルポインタ	*/
	int64_t	nFileSize;		/*!< ファイルサイズ		*/
} Cat_StreamFile_Priv;

//! 読み込む
/*!
	@param[in]	pStream	ストリーム
	@param[out]	pvData	読み込みバッファ
	@param[in]	nSize	読み込むサイズ(バイト単位)
	@reutrn	読み込んだサイズ。負の場合はエラー
*/
static int64_t
Cat_StreamFile_Read( Cat_Stream* pStream, void* pvData, int64_t nSize )
{
	return fread( pvData, 1, nSize, ((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile );
}

//! 書き込む
/*!
	@param[in]	pStream	ストリーム
	@param[in]	pvData	書き込むバッファ
	@param[in]	nSize	書き込むサイズ(バイト単位)
	@reutrn	書き込んだサイズ。負の場合はエラー
*/
static int64_t
Cat_StreamFile_Write( Cat_Stream* pStream, const void* pvData, int64_t nSize )
{
	return fwrite( pvData, 1, nSize, ((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile );
}

//! 位置を設定する
static int64_t
Cat_StreamFile_Seek( Cat_Stream* pStream, int64_t nOffset )
{
	return fseek( ((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile, nOffset, SEEK_SET );
}

//! 閉じる
static void
Cat_StreamFile_Close( Cat_Stream* pStream )
{
	if(pStream) {
		if(pStream->pvPrivate) {
			if(((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile) {
				fclose( ((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile );
				((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile = 0;
			}
			CAT_FREE( pStream->pvPrivate );
			pStream->pvPrivate = 0;
		}
		CAT_FREE( pStream );
	}
}

//! 位置を取得する
/*!
	@param[in]	pStream
	@reutrn	位置
*/
static int64_t
Cat_StreamFile_Tell( Cat_Stream* pStream )
{
	return ftell( ((Cat_StreamFile_Priv*)pStream->pvPrivate)->pFile );
}

//! サイズを取得する
/*!
	@param[in]	pStream
	@reutrn	サイズ(バイト単位)
*/
static int64_t
Cat_StreamFile_GetSize( struct _Cat_Stream* pStream )
{
	return ((Cat_StreamFile_Priv*)pStream->pvPrivate)->nFileSize;
}

//! 読み込み専用でファイルを開く
/*!
	@param[in]	ファイル名
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
Cat_Stream*
Cat_StreamFileReadOpen( const char* pszFilename )
{
	Cat_Stream* rc;
	FILE* fp;
	if((pszFilename == 0) || (pszFilename[0] == 0)) {
		/* 引数が変 */
		return 0;
	}
	fp = fopen( pszFilename, "rb" );
	if(fp == 0) {
		/* 読み込む失敗 */
		return 0;
	}
	rc = (Cat_Stream*)CAT_MALLOC( sizeof(Cat_Stream) );
	if(rc == 0) {
		/* メモリ確保失敗 */
		fclose( fp );
		return 0;
	}
	memset( rc, 0, sizeof(Cat_Stream) );
	rc->pvPrivate = CAT_MALLOC( sizeof(Cat_StreamFile_Priv) );
	if(rc->pvPrivate == 0) {
		/* メモリ確保失敗 */
		fclose( fp );
		CAT_FREE( rc );
		return 0;
	}

	memset( rc->pvPrivate, 0, sizeof(Cat_StreamFile_Priv) );
	((Cat_StreamFile_Priv*)rc->pvPrivate)->pFile = fp;

	rc->Read    = Cat_StreamFile_Read;
	rc->Seek    = Cat_StreamFile_Seek;
	rc->Close   = Cat_StreamFile_Close;
	rc->Tell    = Cat_StreamFile_Tell;
	rc->GetSize = Cat_StreamFile_GetSize;

	// ファイルサイズを調べておく
	fseek( fp, 0, SEEK_END );
	((Cat_StreamFile_Priv*)rc->pvPrivate)->nFileSize = (int64_t)ftell( fp );
	fseek( fp, 0, SEEK_SET );

	return rc;
}

//! 書き込み専用でファイルを開く
/*!
	@param[in]	ファイル名
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
Cat_Stream*
Cat_StreamFileWriteOpen( const char* pszFilename )
{
	Cat_Stream* rc;
	FILE* fp;
	if((pszFilename == 0) || (pszFilename[0] == 0)) {
		/* 引数が変 */
		return 0;
	}
	fp = fopen( pszFilename, "rb" );
	if(fp == 0) {
		/* 読み込む失敗 */
		return 0;
	}
	rc = (Cat_Stream*)CAT_MALLOC( sizeof(Cat_Stream) );
	if(rc == 0) {
		/* メモリ確保失敗 */
		fclose( fp );
		return 0;
	}
	memset( rc, 0, sizeof(Cat_Stream) );
	rc->pvPrivate = CAT_MALLOC( sizeof(Cat_StreamFile_Priv) );
	if(rc->pvPrivate == 0) {
		/* メモリ確保失敗 */
		fclose( fp );
		CAT_FREE( rc );
		return 0;
	}

	memset( rc->pvPrivate, 0, sizeof(Cat_StreamFile_Priv) );
	((Cat_StreamFile_Priv*)rc->pvPrivate)->pFile = fp;

	rc->Write   = Cat_StreamFile_Write;
	rc->Seek    = Cat_StreamFile_Seek;
	rc->Close   = Cat_StreamFile_Close;
	rc->Tell    = Cat_StreamFile_Tell;

	return rc;
}
