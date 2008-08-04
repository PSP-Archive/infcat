//! @file	Cat_StreamPad.c
// メモリストリーム関連

#include <stdint.h>
#include "Cat_StreamPad.h"
#include <string.h>
#include <malloc.h>	// for memalign
#include <pspctrl.h>

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
	SceCtrlData pad;
	uint32_t nRemain;
	uint32_t nData;
} Cat_StreamPad_Priv;

//! 読み込む
/*!
	@param[in]	pStream	ストリーム
	@param[out]	pvData	読み込みバッファ
	@param[in]	nSize	読み込むサイズ(バイト単位)
	@reutrn	読み込んだサイズ。負の場合はエラー
*/
static int64_t
Cat_StreamPad_Read( Cat_Stream* pStream, void* pvData, int64_t nSize )
{
	Cat_StreamPad_Priv* pPrivate = (Cat_StreamPad_Priv*)pStream->pvPrivate;
	uint8_t* pbData = (uint8_t*)pvData;
	int64_t rc = nSize;

	while(nSize > 0) {
		if(pPrivate->nRemain) {
			if(pPrivate->nRemain & 1) {
				*pbData = pPrivate->nData >> 8;
			} else {
				*pbData = pPrivate->nData & 0xFF;
			}
			pPrivate->nRemain--;
			pbData++;
			nSize--;
		} else {
			sceCtrlReadBufferPositive( &pPrivate->pad, 1 );
			pPrivate->nData = (uint32_t)pPrivate->pad.Buttons;
			pPrivate->nRemain = 2;
		}
	}
	return rc;
}

//! 閉じる
static void
Cat_StreamPad_Close( Cat_Stream* pStream )
{
	if(pStream) {
		Cat_StreamPad_Priv* pPrivate = (Cat_StreamPad_Priv*)pStream->pvPrivate;
		if(pPrivate) {
			CAT_FREE( pPrivate );
			pStream->pvPrivate = 0;
		}
		CAT_FREE( pStream );
	}
}

//! パッドストリームを開く
/*!
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
Cat_Stream*
Cat_StreamPadReadOpen( void )
{
	Cat_Stream* rc;
	rc = (Cat_Stream*)CAT_MALLOC( sizeof(Cat_Stream) );
	if(rc == 0) {
		/* メモリ確保失敗 */
		return 0;
	}
	memset( rc, 0, sizeof(Cat_Stream) );
	rc->pvPrivate = CAT_MALLOC( sizeof(Cat_StreamPad_Priv) );
	if(rc->pvPrivate == 0) {
		/* メモリ確保失敗 */
		CAT_FREE( rc );
		return 0;
	}

	memset( rc->pvPrivate, 0, sizeof(Cat_StreamPad_Priv) );

	rc->Read    = Cat_StreamPad_Read;
	rc->Close   = Cat_StreamPad_Close;

	return rc;
}
