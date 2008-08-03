//! @file	Cat_ImageLoader.c
// 画像の読み込み

#include "Cat_ImageLoader.h"

#ifdef USE_CAT_IMAGELOADER_PNG
#include "Cat_ImageLoaderPNG.h"
#endif // USE_CAT_IMAGELOADER_PNG

#ifdef USE_CAT_IMAGELOADER_PCX
#include "Cat_ImageLoaderPCX.h"
#endif // USE_CAT_IMAGELOADER_PCX

#ifdef USE_CAT_IMAGELOADER_JPG
#include "Cat_ImageLoaderJPG.h"
#endif // USE_CAT_IMAGELOADER_JPG

//! 画像を読み込んで、テクスチャを作成する
/*!
	@param[in]	pStream	ストリーム
	@return	作成されたテクスチャ \n
			失敗した場合は、0が返る
*/
Cat_Texture*
Cat_LoadImage( Cat_Stream* pStream )
{
	if(pStream == 0) {
		return 0;
	}

#ifdef USE_CAT_IMAGELOADER_JPG
	if(Cat_ImageLoaderCheckJPG( pStream )) {
		return Cat_ImageLoaderLoadJPG( pStream );
	}
#endif // USE_CAT_IMAGELOADER_JPG

#ifdef USE_CAT_IMAGELOADER_PNG
	if(Cat_ImageLoaderCheckPNG( pStream )) {
		return Cat_ImageLoaderLoadPNG( pStream );
	}
#endif // USE_CAT_IMAGELOADER_PNG

#ifdef USE_CAT_IMAGELOADER_PCX
	if(Cat_ImageLoaderCheckPCX( pStream )) {
		return Cat_ImageLoaderLoadPCX( pStream );
	}
#endif // USE_CAT_IMAGELOADER_PCX

	return 0;
}
