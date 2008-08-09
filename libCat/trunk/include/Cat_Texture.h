//! @file	Cat_Texture.h
// テクスチャ関連

#ifndef INCL_Cat_Texture_h
#define INCL_Cat_Texture_h

#include <stdint.h>
#include "Cat_Palette.h"

#ifdef __cplusplus
extern "C" {
#endif

//! ピクセルフォーマット
typedef enum {
	FORMAT_PIXEL_5650 = 0,		/*!< 16bit RGBA5650	*/
	FORMAT_PIXEL_5551 = 1,		/*!< 16bit RGBA5551	*/
	FORMAT_PIXEL_4444 = 2,		/*!< 16bit RGBA4444	*/
	FORMAT_PIXEL_8888 = 3,		/*!< 32bit RGBA8888	*/
	FORMAT_PIXEL_CLUT4 = 4,		/*!< 4bit Clut		*/
	FORMAT_PIXEL_CLUT8 = 5,		/*!< 8bit Clut		*/

	FORMAT_PIXEL_MAX			/*!< 最大値			*/
} FORMAT_PIXEL;

//! テクスチャ構造体
typedef struct {
	uint32_t		nOriginalWidth;		/*!< オリジナルの横幅				*/
	uint32_t		nOriginalHeight;	/*!< オリジナルの高さ				*/
	uint32_t		nTextureWidth;		/*!< 変換後のテクスチャの横幅		*/
	uint32_t		nTextureHeight;		/*!< 変換後のテクスチャの高さ		*/
//
	uint32_t		nWidth;				/*!< 内部で管理している横幅			*/
	uint32_t		nHeight;			/*!< 内部で管理している高さ			*/
	uint32_t		nPitch;				/*!< ピッチ							*/
	FORMAT_PIXEL	ePixelFormat;		/*!< ピクセルフォーマット			*/
	uint32_t		nTexMode;			/*!< テクスチャモード				*/
	uint32_t		nWidth2;			/*!< 横幅2の乗数					*/
	uint32_t		nHeight2;			/*!< 縦幅2の乗数					*/
	uint32_t		nWidth16;			/*!< 縦幅16バイト単位				*/
	void*			pvData;				/*!< ピクセルデータ					*/
	float			fScaleWidth;		/*!< 横スケール						*/
	float			fScaleHeight;		/*!< 縦スケール						*/
	Cat_Palette*	pPalette;			/*!< パレット						*/
	int32_t			tbl4to8[16];		/*!< 変換テーブル					*/
	Cat_Palette*	pPalette4;			/*!< 4bitパレット					*/
} Cat_Texture;

//! テクスチャ作成
/*!
	\a pvImage は、mallocで確保したメモリを渡すこと。 \n
	Cat_TextureRelease()で解放されるので、解放しないこと。

	@param[in]	nWidth			テクスチャの横幅(ピクセル単位)
	@param[in]	nHeight			テクスチャの高さ(ピクセル単位)
	@param[in]	nPitch			テクスチャの横幅のピッチ(バイト単位)
	@param[in]	pvImage			テクスチャのデータ(mallocで確保したメモリを渡すこと。)
	@param[in]	ePixelFormat	ピクセルフォーマット
	@param[in]	pPalette		パレット
	@return	作成されたテクスチャ。失敗した場合は0が返る。
	@see	Cat_TextureRelease()
*/
extern Cat_Texture* Cat_TextureCreate( uint32_t nWidth, uint32_t nHeight, uint32_t nPitch, void* pvImage, FORMAT_PIXEL ePixelFormat, Cat_Palette* pPalette );

//! テクスチャ解放
/*!
	@param[in]	pTexture	解放するテクスチャ
	@see	Cat_TextureCreate()
*/
extern void Cat_TextureRelease( Cat_Texture* pTexture );

//! テクスチャ設定
/*!
	パレットがある場合は、パレットも設定される。
	@param[in]	pTexture	設定するテクスチャ
*/
extern void Cat_TextureSetTexture( Cat_Texture* pTexture );

//! 横幅を取得
/*!
	@param[in]	pTexture	テクスチャ
	@return	テクスチャの横幅(ピクセル単位)
*/
extern uint32_t Cat_TextureGetWidth( Cat_Texture* pTexture );

//! 縦幅を取得
/*!
	@param[in]	pTexture	テクスチャ
	@return	テクスチャの高さ(ピクセル単位)
*/
extern uint32_t Cat_TextureGetHeight( Cat_Texture* pTexture );

//! ピッチを取得
/*!
	@param[in]	pTexture	テクスチャ
	@return	ピッチ(バイト単位)
*/
extern uint32_t Cat_TextureGetPitch( Cat_Texture* pTexture );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_Texture_h
