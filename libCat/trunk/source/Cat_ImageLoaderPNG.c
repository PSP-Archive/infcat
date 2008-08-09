//! @file	Cat_ImageLoaderPNG.c
// PNG形式の読み込み
// zlibとlibpngが必要

#ifdef USE_CAT_IMAGELOADER_PNG

#include <malloc.h>	// for memalign
#include "Cat_Texture.h"
#include "Cat_Stream.h"
#include "png.h"

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! PNG形式かどうかを調べるために読み込むサイズ
#define PNG_BYTES_TO_CHECK (4)

//! フォーマットのチェック
/*!
	@param[in]	pStream ストリーム
	@return	対応形式なら1 \n
			違うなら0を返す。
*/
int32_t
Cat_ImageLoaderCheckPNG( Cat_Stream* pStream )
{
	int32_t rc = 0;
	uint8_t sig[PNG_BYTES_TO_CHECK];
	int64_t pos;

	if(pStream == 0) {
		return 0;
	}

	pos = Cat_StreamTell( pStream );
	if(pos >= 0) {
		if(Cat_StreamRead( pStream, sig, PNG_BYTES_TO_CHECK ) == PNG_BYTES_TO_CHECK) {
			rc = png_check_sig( sig, PNG_BYTES_TO_CHECK ) ? 1 : 0;
		}
		Cat_StreamSeek( pStream, pos );
	}
	return rc;
}

//! ストリームから読み込む
static void
user_read_data( png_structp png_ptr, png_bytep data, png_size_t length )
{
	Cat_Stream* pStream = (Cat_Stream*)png_get_io_ptr( png_ptr );
	//! @todo エラー処理はどうするん？
	Cat_StreamRead( pStream, data, length );
}

//! PNG形式を読み込んでテクスチャを作成する
/*!
	@param[in]	pStream	ストリーム
	@return	作成されたテクスチャ \n
			失敗した場合は、0が返る
*/
Cat_Texture*
Cat_ImageLoaderLoadPNG( Cat_Stream* pStream )
{
	Cat_Texture* rc;
	png_structp png_ptr;
	png_infop info_ptr;
	png_infop info_end;
	unsigned long nWidth;
	unsigned long nHeight;
	int bit_depth;
	int color_type;
	int interlace_type;
	unsigned char **imageRow;
	unsigned char *image;
	unsigned long i;
	int fNeedUpdate;

	if(pStream == 0) {
		return 0;
	}

	// 初期化
	png_ptr = png_create_read_struct( PNG_LIBPNG_VER_STRING, 0, 0, 0 );
	if(png_ptr == 0) {
		return 0;
	}
	info_ptr = png_create_info_struct( png_ptr );
	if(info_ptr == 0) {
		png_destroy_read_struct( &png_ptr, (png_infopp)0, (png_infopp)0 );
		return 0;
	}
	info_end = png_create_info_struct( png_ptr );
	if(info_end == 0) {
		png_destroy_read_struct( &png_ptr, &info_ptr, (png_infopp)0 );
		return 0;
	}

	// ioをCat_Streamを使うように変更
	png_set_read_fn( png_ptr, pStream, user_read_data );

	// 情報を読み込んで、色々と読み込みの設定
	png_read_info( png_ptr, info_ptr );
	png_get_IHDR( png_ptr, info_ptr, &nWidth, &nHeight, &bit_depth, &color_type, &interlace_type, NULL, NULL );

	fNeedUpdate = 0;
	if(bit_depth == 16) {
		// 色深度が16bitなら8bitへ変換
		png_set_strip_16( png_ptr );
		fNeedUpdate = 1;
	} else if(bit_depth < 8) {
		// 1,2,4bitならパックしたままにする
		png_set_packing( png_ptr );
		fNeedUpdate = 1;
	}
	if(color_type == PNG_COLOR_TYPE_RGB || (color_type == PNG_COLOR_TYPE_GRAY)) {
		// RGBならαを追加して32bitにする
		png_uint_32 filler = 0xFF;
		png_set_add_alpha( png_ptr, filler, PNG_FILLER_AFTER );
		fNeedUpdate = 1;
	}
	if(fNeedUpdate) {
		png_read_update_info( png_ptr, info_ptr );
	}

	// メモリ確保
	image = (unsigned char*)CAT_MALLOC( nHeight * png_get_rowbytes( png_ptr, info_ptr ) );
	if(image == 0) {
		png_destroy_read_struct( &png_ptr, &info_ptr, &info_end );
		return 0;
	}
	imageRow = (unsigned char**)CAT_MALLOC( nHeight * sizeof(png_bytep) );
	if(imageRow == 0) {
		CAT_FREE( image );
		png_destroy_read_struct( &png_ptr, &info_ptr, &info_end );
		return 0;
	}
	for(i = 0; i < nHeight; i++) {
		imageRow[i] = &image[i * png_get_rowbytes( png_ptr, info_ptr )];
	}

	// イメージ読み込み
	png_read_image( png_ptr, imageRow );

	// テクスチャへ変換
	rc = 0;
	if(color_type == PNG_COLOR_TYPE_PALETTE) {
		// パレットの読み込み
		png_colorp palette;
		int num_palette;
		png_get_PLTE( png_ptr, info_ptr, &palette, &num_palette );
		switch(bit_depth) {
			case 4:
				{
					Cat_Palette* pPalette;
					uint8_t pbColorMap[16*4];
					for(i = 0; i < num_palette; i++) {
						pbColorMap[i * 4 + 0] = palette[i].red;
						pbColorMap[i * 4 + 1] = palette[i].green;
						pbColorMap[i * 4 + 2] = palette[i].blue;
						pbColorMap[i * 4 + 3] = 0xFF;
					}
					pPalette = Cat_PaletteCreate( FORMAT_PALETTE_8888, 16, pbColorMap );
					if(pPalette) {
						rc = Cat_TextureCreate( nWidth, nHeight, png_get_rowbytes( png_ptr, info_ptr ), image, FORMAT_PIXEL_CLUT4, pPalette );
						Cat_PaletteRelease( pPalette );
					}
					break;
				}
			case 8:
				{
					Cat_Palette* pPalette;
					uint8_t pbColorMap[256*4];	// スタック注意
					for(i = 0; i < num_palette; i++) {
						pbColorMap[i * 4 + 0] = palette[i].red;
						pbColorMap[i * 4 + 1] = palette[i].green;
						pbColorMap[i * 4 + 2] = palette[i].blue;
						pbColorMap[i * 4 + 3] = 0xFF;
					}
					pPalette = Cat_PaletteCreate( FORMAT_PALETTE_8888, 256, pbColorMap );
					if(pPalette) {
						rc = Cat_TextureCreate( nWidth, nHeight, png_get_rowbytes( png_ptr, info_ptr ), image, FORMAT_PIXEL_CLUT4, pPalette );
						Cat_PaletteRelease( pPalette );
					}
					break;
				}
				break;
		}
	} else if((color_type == PNG_COLOR_TYPE_RGB) || (color_type == PNG_COLOR_TYPE_RGB_ALPHA)) {
		rc = Cat_TextureCreate( nWidth, nHeight, png_get_rowbytes( png_ptr, info_ptr ), image, FORMAT_PIXEL_8888, 0 );
	}

	// 後始末
	CAT_FREE( image );
	CAT_FREE( imageRow );
	png_destroy_read_struct( &png_ptr, &info_ptr, &info_end );
	return rc;
}

#endif	// USE_CAT_IMAGELOADER_PNG
