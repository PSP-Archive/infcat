//! @file	Cat_ImageLoader.h
// 画像の読み込み

#ifndef INCL_Cat_ImageLoader_h
#define INCL_Cat_ImageLoader_h

#include <stdint.h>
#include "Cat_Texture.h"
#include "Cat_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

//! 画像を読み込んで、テクスチャを作成する
/*!
	@param[in]	pStream	ストリーム
	@return	作成されたテクスチャ \n
			失敗した場合は、0が返る
*/
extern Cat_Texture* Cat_LoadImage( Cat_Stream* pStream );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_ImageLoader_h
