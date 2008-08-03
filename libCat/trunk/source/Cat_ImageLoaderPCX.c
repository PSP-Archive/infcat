//! @file	Cat_ImageLoaderPCX.c
// PCX形式の読み込み

#ifdef USE_CAT_IMAGELOADER_PCX

#include <malloc.h>	// for memalign
#include "Cat_Texture.h"
#include "Cat_Stream.h"

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! フォーマットのチェック
/*!
	@param[in]	pStream ストリーム
	@return	対応形式なら1 \n
			違うなら0を返す。
*/
int32_t
Cat_ImageLoaderCheckPCX( Cat_Stream* pStream )
{
	return 0;
}

//! PCX形式を読み込んでテクスチャを作成する
/*!
	@param[in]	pStream	ストリーム
	@return	作成されたテクスチャ \n
			失敗した場合は、0が返る
*/
Cat_Texture*
Cat_ImageLoaderLoadPCX( Cat_Stream* pStream )
{
	return 0;
}

#endif	// USE_CAT_IMAGELOADER_PCX
