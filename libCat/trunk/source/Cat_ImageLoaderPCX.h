//! @file	Cat_ImageLoaderPCX.h
// PCX形式の読み込み

#ifndef INC_ImageLoaderPCX_h
#define INC_ImageLoaderPCX_h

#include <stdint.h>
#include "Cat_Texture.h"
#include "Cat_Stream.h"

#ifdef USE_CAT_IMAGELOADER_PCX

#ifdef __cplusplus
extern "C" {
#endif

//! フォーマットのチェック
/*!
	@param[in]	pStream ストリーム
	@return	対応形式なら1 \n
			違うなら0を返す。
*/
extern int32_t Cat_ImageLoaderCheckPCX( Cat_Stream* pStream );

//! PCX形式を読み込んでテクスチャを作成する
/*!
	@param[in]	pStream	ストリーム
	@return	作成されたテクスチャ \n
			失敗した場合は、0が返る
*/
extern Cat_Texture* Cat_ImageLoaderLoadPCX( Cat_Stream* pStream );

#ifdef __cplusplus
}
#endif

#endif // USE_CAT_IMAGELOADER_PCX

#endif // INC_ImageLoaderPCX_h
