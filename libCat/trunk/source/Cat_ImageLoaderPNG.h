//! @file	Cat_ImageLoaderPNG.h
// PNG形式の読み込み
// zlibとlibpngが必要

#ifndef INC_ImageLoaderPNG_h
#define INC_ImageLoaderPNG_h

#include <stdint.h>
#include "Cat_Texture.h"
#include "Cat_Stream.h"

#ifdef USE_CAT_IMAGELOADER_PNG

#ifdef __cplusplus
extern "C" {
#endif

//! フォーマットのチェック
/*!
	@param[in]	pStream ストリーム
	@return	対応形式なら1 \n
			違うなら0を返す。
*/
extern int32_t Cat_ImageLoaderCheckPNG( Cat_Stream* pStream );

//! PNG形式を読み込んでテクスチャを作成する
/*!
	@param[in]	pStream	ストリーム
	@return	作成されたテクスチャ \n
			失敗した場合は、0が返る
*/
extern Cat_Texture* Cat_ImageLoaderLoadPNG( Cat_Stream* pStream );

#ifdef __cplusplus
}
#endif

#endif // USE_CAT_IMAGELOADER_PNG

#endif // INC_ImageLoaderPNG_h
