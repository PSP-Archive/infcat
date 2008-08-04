//! @file	Cat_StreamMemory.c
// メモリストリーム関連

#include "Cat_StreamMemory.h"
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
	int64_t		nSize;			/*!< サイズ									*/
	int64_t		nPosition;		/*!< 位置									*/
	uint8_t*	pbData;			/*!< データ									*/
	int32_t		fFree;			/*!< クローズ時にデータを解放するかどうか	*/
} Cat_StreamMemory_Priv;

//! 読み込む
/*!
	@param[in]	pStream	ストリーム
	@param[out]	pvData	読み込みバッファ
	@param[in]	nSize	読み込むサイズ(バイト単位)
	@reutrn	読み込んだサイズ。負の場合はエラー
*/
static int64_t
Cat_StreamMemory_Read( Cat_Stream* pStream, void* pvData, int64_t nSize )
{
	Cat_StreamMemory_Priv* pPrivate = (Cat_StreamMemory_Priv*)pStream->pvPrivate;

	int64_t nRemain = pPrivate->nSize - pPrivate->nPosition;

	if(nRemain < nSize) {
		nSize = nRemain;
	}
	if(nSize > 0) {
		memcpy( pvData, pPrivate->pbData + pPrivate->nPosition, nSize );
		pPrivate->nPosition += nSize;
	}
	return nSize;
}

//! 書き込む
/*!
	@param[in]	pStream	ストリーム
	@param[in]	pvData	書き込むバッファ
	@param[in]	nSize	書き込むサイズ(バイト単位)
	@reutrn	書き込んだサイズ。負の場合はエラー
*/
#if 0
static int64_t
Cat_StreamMemory_Write( Cat_Stream* pStream, const void* pvData, int64_t nSize )
{
	Cat_StreamMemory_Priv* pPrivate = (Cat_StreamMemory_Priv*)pStream->pvPrivate;

	int64_t nRemain = pPrivate->nSize - pPrivate->nPosition;

	if(nRemain < nSize) {
		nSize = nRemain;
	}
	if(nSize > 0) {
		memcpy( pPrivate->pbData + pPrivate->nPosition, pvData, nSize );
		pPrivate->nPosition += nSize;
	}
	return nSize;
}
#endif

//! 位置を設定する
static int64_t
Cat_StreamMemory_Seek( Cat_Stream* pStream, int64_t nOffset )
{
	Cat_StreamMemory_Priv* pPrivate = (Cat_StreamMemory_Priv*)pStream->pvPrivate;

	if(nOffset < 0) {
		nOffset = 0;
	}
	if(nOffset < pPrivate->nSize) {
		pPrivate->nPosition = nOffset;
	} else {
		pPrivate->nPosition = pPrivate->nSize - 1;
	}
	return pPrivate->nPosition;
}

//! 閉じる
static void
Cat_StreamMemory_Close( Cat_Stream* pStream )
{
	if(pStream) {
		Cat_StreamMemory_Priv* pPrivate = (Cat_StreamMemory_Priv*)pStream->pvPrivate;
		if(pPrivate) {
			if(pPrivate->fFree && pPrivate->pbData) {
				CAT_FREE( pPrivate->pbData );
			}
			pPrivate->pbData = 0;
			CAT_FREE( pPrivate );
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
Cat_StreamMemory_Tell( Cat_Stream* pStream )
{
	Cat_StreamMemory_Priv* pPrivate = (Cat_StreamMemory_Priv*)pStream->pvPrivate;
	return pPrivate->nPosition;
}

//! サイズを取得する
/*!
	@param[in]	pStream
	@reutrn	サイズ(バイト単位)
*/
static int64_t
Cat_StreamMemory_GetSize( struct _Cat_Stream* pStream )
{
	Cat_StreamMemory_Priv* pPrivate = (Cat_StreamMemory_Priv*)pStream->pvPrivate;
	return pPrivate->nSize;
}

//! 読み込み専用のメモリストリームを開く
/*!
	@param[in]	pvData	データへのポインタ
	@param[in]	nSize	データサイズ
	@param[in]	fFree	クローズ時データを解放するかどうか 0以外で解放する
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
Cat_Stream*
Cat_StreamMemoryReadOpen( void* pvData, int64_t nSize, uint32_t fFree )
{
	Cat_Stream* rc;
	if((pvData == 0) || (nSize <= 0)) {
		/* 引数が変 */
		return 0;
	}
	rc = (Cat_Stream*)CAT_MALLOC( sizeof(Cat_Stream) );
	if(rc == 0) {
		/* メモリ確保失敗 */
		return 0;
	}
	memset( rc, 0, sizeof(Cat_Stream) );
	rc->pvPrivate = CAT_MALLOC( sizeof(Cat_StreamMemory_Priv) );
	if(rc->pvPrivate == 0) {
		/* メモリ確保失敗 */
		CAT_FREE( rc );
		return 0;
	}

	memset( rc->pvPrivate, 0, sizeof(Cat_StreamMemory_Priv) );
	((Cat_StreamMemory_Priv*)rc->pvPrivate)->nSize  = nSize;
	((Cat_StreamMemory_Priv*)rc->pvPrivate)->pbData = (uint8_t*)pvData;
	((Cat_StreamMemory_Priv*)rc->pvPrivate)->fFree  = fFree;

	rc->Read    = Cat_StreamMemory_Read;
	rc->Seek    = Cat_StreamMemory_Seek;
	rc->Close   = Cat_StreamMemory_Close;
	rc->Tell    = Cat_StreamMemory_Tell;
	rc->GetSize = Cat_StreamMemory_GetSize;

	return rc;
}
