//! @file	Cat_Texture.c
// テクスチャ関連

#include <pspgu.h>
#include <psputils.h>
#include <string.h>
#include <malloc.h>	// for memalign
#include "Cat_Texture.h"

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! テクスチャのモード入れ替え無し
#define CAT_TEXMODE_NORMAL (0)
//! テクスチャのモード入れ替えあり
#define CAT_TEXMODE_SWAP   (1)


//! サイズを調整する
static int32_t ConvertSize( Cat_Texture* pTexture );
//! テクスチャ内を入れ替えて、高速で描画できるように変換
static void ConvertImageSwap( Cat_Texture* pTexture );
//! 使っている色を調べて16色以下なら4bitにする
static void Convert4( Cat_Texture* pTexture );

//! 最小の2の乗数に切り上げる
/*!
	@param[in]	x	値(0～65536まで)
	@return	最小の2の乗数
*/
inline uint32_t up2( uint32_t x )
{
	if(x <= 1) {
		return 1;
	} else if(x <= 2) {
		return 2;
	} else if(x <= 4) {
		return 4;
	} else if(x <= 8) {
		return 8;
	} else if(x <= 16) {
		return 16;
	} else if(x <= 32) {
		return 32;
	} else if(x <= 64) {
		return 64;
	} else if(x <= 128) {
		return 128;
	} else if(x <= 256) {
		return 256;
	} else if(x <= 512) {
		return 512;
	} else if(x <= 1024) {
		return 1024;
	} else if(x <= 2048) {
		return 2048;
	} else if(x <= 4096) {
		return 4096;
	} else if(x <= 8192) {
		return 8192;
	} else if(x <= 16384) {
		return 16384;
	} else if(x <= 32768) {
		return 32768;
	} else if(x <= 65536) {
		return 65536;
	}
	return 1024*128;
}

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
Cat_Texture*
Cat_TextureCreate( uint32_t nWidth, uint32_t nHeight, uint32_t nPitch, void* pvImage, FORMAT_PIXEL ePixelFormat, Cat_Palette* pPalette )
{
	Cat_Texture* rc;

	rc = CAT_MALLOC( sizeof(Cat_Texture) );
	if(rc) {
		uint32_t i;
		rc->ePixelFormat = ePixelFormat;
		rc->nOriginalWidth  = nWidth;		// オリジナル
		rc->nOriginalHeight = nHeight;		// オリジナル
		rc->nTextureWidth   = nWidth;
		rc->nTextureHeight  = nHeight;
		rc->nWidth          = nWidth;
		rc->nWidth2         = up2( nWidth );
		rc->pPalette        = pPalette;
		rc->pPalette4       = 0;
		rc->nTexMode        = CAT_TEXMODE_NORMAL;
		if(rc->pPalette) {
			rc->pPalette->nRef++;
		}

		// イメージのメモリ確保と初期化
		rc->nHeight  = (nHeight + 7) & ~7;
		rc->nHeight2 = up2( rc->nHeight );
		rc->nPitch   = (nPitch + 15) & ~15;
		rc->pvData   = CAT_MALLOC( rc->nPitch * rc->nHeight );
		if(rc->pvData == 0) {
			// 駄目だった
			Cat_TextureRelease( rc );
			return 0;
		}
		memset( rc->pvData, 0, rc->nPitch * rc->nHeight );
		for(i = 0; i < nHeight; i++) {
			memcpy( (uint8_t*)rc->pvData + rc->nPitch * i, (uint8_t*)pvImage + nPitch * i, nPitch );
		}

		// テクスチャサイズが大きかったら小さくする
		if((rc->nWidth > 512) || (rc->nHeight > 512)) {
			if(!ConvertSize( rc )) {
				// 駄目だった
				Cat_TextureRelease( rc );
				return 0;
			}
		}

		// 使っている色を調べて16色以下なら4bitにする
		Convert4( rc );

		// テクスチャスケーリング
		rc->fScaleWidth  = (float)rc->nWidth  / (float)rc->nWidth2;
		rc->fScaleHeight = (float)rc->nHeight / (float)rc->nHeight2;
		// 横幅
		switch(rc->ePixelFormat) {
			case FORMAT_PIXEL_8888:
				rc->nWidth16 = rc->nPitch / 4;
				break;
			case FORMAT_PIXEL_5650:
			case FORMAT_PIXEL_5551:
			case FORMAT_PIXEL_4444:
				rc->nWidth16 = rc->nPitch / 2;
				break;
			case FORMAT_PIXEL_CLUT8:
			default:
				rc->nWidth16 = rc->nPitch;
				break;
			case FORMAT_PIXEL_CLUT4:
				rc->nWidth16 = rc->nPitch * 2;
				break;
		}
		// イメージスワップ
		ConvertImageSwap( rc );

		// キャッシュを吐き出して、イメージデータ部分のキャッシュを無効に
		// テクスチャは、基本的に作ったら変更しないので
		sceKernelDcacheWritebackInvalidateRange( rc->pvData, rc->nHeight * rc->nPitch );
	}
	return rc;
}

//! テクスチャ解放
/*!
	@param[in]	pTexture	解放するテクスチャ
	@see	Cat_TextureCreate()
*/
void
Cat_TextureRelease( Cat_Texture* pTexture )
{
	if(pTexture == 0) {
		return;
	}

	if(pTexture->pPalette) {
		// パレット解放
		Cat_PaletteRelease( pTexture->pPalette );
		pTexture->pPalette = 0;
	}
	if(pTexture->pPalette4) {
		// パレット解放
		Cat_PaletteRelease( pTexture->pPalette4 );
		pTexture->pPalette4 = 0;
	}
	if(pTexture->pvData) {
		// イメージ解放
		CAT_FREE( pTexture->pvData );
		pTexture->pvData = 0;
	}
	CAT_FREE( pTexture );
}

//! テクスチャ設定
/*!
	パレットがある場合は、パレットも設定される。
	@param[in]	pTexture	設定するテクスチャ
*/
void
Cat_TextureSetTexture( Cat_Texture* pTexture )
{
	if(pTexture && pTexture->pvData) {
		/* テクスチャ有効 */
		sceGuEnable( GU_TEXTURE_2D );

		/* テクスチャ設定 */
		sceGuTexMode( (int)pTexture->ePixelFormat, 0, 0, pTexture->nTexMode );
		sceGuTexImage( 0, pTexture->nWidth2, pTexture->nHeight2, pTexture->nWidth16, pTexture->pvData );
		sceGuTexFlush();

		/* 2の乗数じゃないとき用の処理 */
		sceGuTexScale( pTexture->fScaleWidth, pTexture->fScaleHeight );
		/* sceGuTexOffset( 0.0f, 0.0f ); */

		/* パレット設定 */
		if(pTexture->pPalette) {
			if(pTexture->pPalette4) {
				// 8bitから4ビットへ変換されているテクスチャ
				// 元のパレットから再構成する
				int i;
				if(pTexture->pPalette->ePaletteFormat == FORMAT_PALETTE_8888) {
					for(i = 0; i < 16; i++) {
						((uint32_t*)pTexture->pPalette4->pvData)[i] = ((uint32_t*)pTexture->pPalette->pvData)[pTexture->tbl4to8[i]];
					}
				} else {
					for(i = 0; i < 16; i++) {
						((uint16_t*)pTexture->pPalette4->pvData)[i] = ((uint16_t*)pTexture->pPalette->pvData)[pTexture->tbl4to8[i]];
					}
				}
				Cat_PaletteSetPalette( pTexture->pPalette4 );
			} else {
				Cat_PaletteSetPalette( pTexture->pPalette );
			}
		}
	} else {
		sceGuDisable( GU_TEXTURE_2D );	/* テクスチャ無効 */
	}
}

//! 横幅を取得
/*!
	@param[in]	pTexture	テクスチャ
	@return	テクスチャの横幅(ピクセル単位)
*/
uint32_t
Cat_TextureGetWidth( Cat_Texture* pTexture )
{
	return pTexture->nOriginalWidth;
}

//! 縦幅を取得
/*!
	@param[in]	pTexture	テクスチャ
	@return	テクスチャの高さ(ピクセル単位)
*/
uint32_t
Cat_TextureGetHeight( Cat_Texture* pTexture )
{
	return pTexture->nOriginalHeight;
}

//! ピッチを取得
/*!
	@param[in]	pTexture	テクスチャ
	@return	ピッチ(バイト単位)
*/
uint32_t
Cat_TextureGetPitch( Cat_Texture* pTexture )
{
	return pTexture->nPitch;
}

//! サイズを調整する
static int32_t
ConvertSize( Cat_Texture* pTexture )
{
	uint32_t dx;
	uint32_t dy;
	uint32_t nWidth2;
	uint32_t nHeight2;
	uint8_t* work;
	uint32_t h;
	uint32_t w;
	uint32_t pitch;
	uint32_t ppb;
	uint32_t x;
	uint32_t y;
	uint32_t yy;
	uint32_t xx;

	if(pTexture == 0) {
		return 0;
	}

	nWidth2  = up2( pTexture->nWidth );
	nHeight2 = up2( pTexture->nHeight );
	if(nWidth2 <= 512) {
		dx = 1;
	} else if(nWidth2 <= 512*2) {
		dx = 2;
	} else if(nWidth2 <= 512*4) {
		dx = 4;
	} else if(nWidth2 <= 512*8) {
		dx = 8;
	} else if(nWidth2 <= 512*16) {
		dx = 16;
	} else if(nWidth2 <= 512*32) {
		dx = 32;
	} else if(nWidth2 <= 512*64) {
		dx = 64;
	} else if(nWidth2 <= 512*128) {
		dx = 128;
	} else if(nWidth2 <= 512*256) {
		dx = 256;
	} else if(nWidth2 <= 512*512) {
		dx = 512;
	} else {
		return 0;	/* さすがに無理 */
	}

	if(nHeight2 <= 512) {
		dy = 1;
	} else if(nHeight2 <= 512*2) {
		dy = 2;
	} else if(nHeight2 <= 512*4) {
		dy = 4;
	} else if(nHeight2 <= 512*8) {
		dy = 8;
	} else if(nHeight2 <= 512*16) {
		dy = 16;
	} else if(nHeight2 <= 512*32) {
		dy = 32;
	} else if(nHeight2 <= 512*64) {
		dy = 64;
	} else if(nHeight2 <= 512*128) {
		dy = 128;
	} else if(nHeight2 <= 512*256) {
		dy = 256;
	} else if(nHeight2 <= 512*512) {
		dy = 512;
	} else {
		return 0;	/* さすがに無理 */
	}

	/* 縮小した情報へ */
	h = (((pTexture->nHeight / dy) + 7) & ~7);		/* 8の倍数に */
	w = pTexture->nWidth / dx;						/* 横幅 */
	pitch = 1;
	ppb = 0;
	/* 16バイト単位に */
	switch(pTexture->ePixelFormat) {
		case FORMAT_PIXEL_CLUT4:
			w = ((w * 4 + 127) & ~127) / 4;
			pitch = w / 2;
			break;
		case FORMAT_PIXEL_CLUT8:
		default:
			w = ((w + 15) & ~15);
			pitch = w;
			ppb = 1;
			break;
		case FORMAT_PIXEL_5650:
		case FORMAT_PIXEL_5551:
		case FORMAT_PIXEL_4444:
			w = ((w * 2 + 15) & ~15) / 2;
			pitch = w * 2;
			ppb = 2;
			break;
		case FORMAT_PIXEL_8888:
			w = ((w * 4 + 15) & ~15) / 4;
			pitch = w * 4;
			ppb = 4;
			break;
	}
	/* テクスチャ縮小 */
	work = (uint8_t*)CAT_MALLOC( pitch * h );
	if(work == 0) {
		return 0;
	}
	memset( work, 0, pitch * h );
	if(pTexture->ePixelFormat == FORMAT_PIXEL_CLUT4) {
		yy = 0;
		for(y = 0; y < pTexture->nHeight;) {
			xx = 0;
			for(x = 0; x < pTexture->nWidth; x += dx) {
				uint8_t c = *((uint8_t*)pTexture->pvData + x / 2 + y * pTexture->nPitch);
				if(x & 1) {
					c &= 0xf;
				} else {
					c >>= 4;
				}
				if(xx & 1) {
					/* 奇数 */
					work[xx / 2 + yy * pitch] = (work[xx / 2 + yy * pitch] & 0xf0) | c;
				} else {
					work[xx / 2 + yy * pitch] = (work[xx / 2 + yy * pitch] & 0xf) | (c << 4);
				}
				xx++;
			}
			yy++;
			y += dy;
		}
	} else {
		yy = 0;
		for(y = 0; y < pTexture->nHeight;) {
			xx = 0;
			for(x = 0; x < pTexture->nWidth; x += dx) {
				memcpy( &work[xx * ppb + yy * pitch], ((uint8_t*)pTexture->pvData + x * ppb + y * pTexture->nPitch), ppb );
				xx++;
			}
			yy++;
			y += dy;
		}
	}
	CAT_FREE( pTexture->pvData );
	pTexture->pvData   = (void*)work;
	pTexture->nPitch   = pitch;
	pTexture->nWidth   = w;
	pTexture->nHeight  = h;
	pTexture->nWidth2  = up2( pTexture->nWidth );
	pTexture->nHeight2 = up2( pTexture->nHeight );
	pTexture->nTextureWidth  = pTexture->nOriginalWidth  / dx;
	pTexture->nTextureHeight = pTexture->nOriginalHeight / dy;
	return 1;
}

//! テクスチャ内を入れ替えて、高速で描画できるように変換
/*!
	高さは8の倍数でないと変換されない
	@param[in,out]	pTexture	テクスチャ
*/
static void
ConvertImageSwap( Cat_Texture* pTexture )
{
	if(pTexture->pvData && (pTexture->nTexMode == CAT_TEXMODE_NORMAL)
		&& ((pTexture->nHeight & 7) == 0)) {
		uint8_t* work = (uint8_t*)CAT_MALLOC( pTexture->nPitch * 8 );
		if(work) {
			uint32_t h = pTexture->nHeight;
			uint8_t* pSrc = (uint8_t*)pTexture->pvData;
			uint32_t y;
			uint32_t x;
			uint32_t yy;
			uint32_t xx;
			for(y = 0; y < h; y += 8) {
				memcpy( work, pSrc, pTexture->nPitch * 8 );
				for(x = 0; x < pTexture->nPitch; x += 16) {
					for(yy = 0; yy < 8; yy++) {
						for(xx = x; xx < x + 16; xx++) {
							*pSrc++ = work[xx + yy * pTexture->nPitch];
						}
					}
				}
			}
			CAT_FREE( work );
			pTexture->nTexMode = CAT_TEXMODE_SWAP;
		}
	}
}

//! 使っている色を調べて16色以下なら4bitにする
/*!
	@param[in,out]	pTexture	テクスチャ
*/
static void
Convert4( Cat_Texture* pTexture )
{
	uint8_t his[256];
	uint32_t x;
	uint32_t y;
	uint32_t count;
	uint8_t* work;

	if((pTexture == 0) || (pTexture->pvData == 0) || (pTexture->pPalette == 0)) {
		return;
	}

	if(pTexture->ePixelFormat != FORMAT_PIXEL_CLUT8) {
		return;
	}

	memset( his, 32, 256 );

	count = 0;
	for(y = 0; y < pTexture->nHeight; y++) {
		for(x = 0; x < pTexture->nWidth; x++) {
			unsigned char c = *((uint8_t*)pTexture->pvData + x + y * pTexture->nPitch);
			if(his[c] == 32) {
				his[c] = count;
				count++;
				if(count > 16) {
					/* 色つかいすぎなので駄目です。 */
					return;
				}
			}
		}
	}

	if(pTexture->pPalette4) {
		Cat_PaletteRelease( pTexture->pPalette4 );
	}
	pTexture->pPalette4 = Cat_PaletteCreate( pTexture->pPalette->ePaletteFormat, 16, 0 );
	if(pTexture->pPalette4 == 0) {
		return;	/* パレット作成に失敗した */
	}

	/* パレット変換テーブル作成 */
	count = 0;
	for(x = 0; x < 256; x++) {
		if(his[x] < 16) {
			his[x] = count;
			pTexture->tbl4to8[count] = x;	/* 変換テーブル */
			count++;
		}
	}

	/* 16色に変換する */
	int h = ((pTexture->nHeight + 7) & ~7);						/* 8の倍数に */
	int pitch = ((pTexture->nWidth * 4 + 127) & ~127) / 8;		/* 16バイトの倍数に */
	work = (uint8_t*)CAT_MALLOC( pitch * h );
	if(work) {
		memset( work, 0, pitch * h );
		for(y = 0; y < pTexture->nHeight; y++) {
			for(x = 0; x < pTexture->nWidth; x++) {
				uint8_t c = *((uint8_t*)pTexture->pvData + x + y * pTexture->nPitch);
				if((x & 1) == 0) {
					work[(x / 2) + y * pitch] = his[c];
				} else {
					work[(x / 2) + y * pitch] |= his[c] << 4;
				}
			}
		}
		CAT_FREE( pTexture->pvData );
		pTexture->pvData       = (void*)work;
		pTexture->nPitch       = pitch;
		pTexture->ePixelFormat = FORMAT_PIXEL_CLUT4;
	} else {
		if(pTexture->pPalette4) {
			Cat_PaletteRelease( pTexture->pPalette4 );
			pTexture->pPalette4 = 0;
		}
	}
}

//! 32bit(RGBA8888)のテクスチャからテクセルを取得する
/*!
	@param[in]	pTexture	テクスチャ
	@param[in]	x			x座標
	@param[in]	y			y座標
	@return	ピクセル値
*/
static uint32_t
Cat_TextureGetPixel32( const Cat_Texture* pTexture, uint32_t x, uint32_t y )
{
	if(pTexture->nTexMode) {
		uint32_t nOffset =
			(y & 7) * 16
			+ (y / 8) * (pTexture->nPitch * 8)
			+ (x / 4) * (16*8)
			+ (x & 3) * 4;
		return *(uint32_t*)((uint8_t*)pTexture->pvData + nOffset);
	} else {
		uint32_t nOffset = x * 4 + y * pTexture->nPitch;
		return *(uint32_t*)((uint8_t*)pTexture->pvData + nOffset);
	}
}

//! 16bit(RGBA5551,RGBA5650,RGBA4444)のテクスチャからテクセルを取得する
/*!
	@param[in]	pTexture	テクスチャ
	@param[in]	x			x座標
	@param[in]	y			y座標
	@return	ピクセル値
*/
static uint16_t
Cat_TextureGetPixel16( Cat_Texture* pTexture, uint32_t x, uint32_t y )
{
	uint32_t nOffset;
	if(pTexture->nTexMode) {
		nOffset =
			(y & 7) * 16
			+ (y / 8) * (pTexture->nPitch * 8)
			+ (x / 8) * (16*8)
			+ (x & 7) * 2;
	} else {
		nOffset = x * 2 + y * pTexture->nPitch;
	}
	return *(uint16_t*)((uint8_t*)pTexture->pvData + nOffset);
}

//! 8bit(CLUT8)のテクスチャからテクセルを取得する
/*!
	@param[in]	pTexture	テクスチャ
	@param[in]	x			x座標
	@param[in]	y			y座標
	@return	ピクセル値
*/
static uint8_t
Cat_TextureGetPixel8( Cat_Texture* pTexture, uint32_t x, uint32_t y )
{
	uint32_t nOffset;
	if(pTexture->nTexMode) {
		nOffset =
			(y & 7) * 16
			+ (y / 8) * (pTexture->nPitch * 8)
			+ (x / 16) * (16*8)
			+ (x & 15);
	} else {
		nOffset = x + y * pTexture->nPitch;
	}
	return *((uint8_t*)pTexture->pvData + nOffset);
}

//! 8bit(CLUT8)のテクスチャからテクセルを取得する
/*!
	@param[in]	pTexture	テクスチャ
	@param[in]	x			x座標
	@param[in]	y			y座標
	@return	ピクセル値
*/
static uint8_t
Cat_TextureGetPixel4( Cat_Texture* pTexture, uint32_t x, uint32_t y )
{
	uint32_t nOffset;
	if(pTexture->nTexMode) {
		nOffset =
			(y & 7) * 16
			+ (y / 8) * (pTexture->nPitch * 8)
			+ (x / 32) * (16*8)
			+ ((x / 2) & 15);
	} else {
		nOffset = (x / 2) + y * pTexture->nPitch;
	}
	if(x & 1) {
		return *((uint8_t*)pTexture->pvData + nOffset) >> 4;
	} else {
		return *((uint8_t*)pTexture->pvData + nOffset) & 0xF;
	}
}

//! テクスチャからテクセルを取得する
/*!
	@param[in]	pTexture	テクスチャ
	@param[in]	x			x座標
	@param[in]	y			y座標
	@return	RGBA8888形式のピクセル値
*/
uint32_t
Cat_TextureGetPixel( Cat_Texture* pTexture, uint32_t x, uint32_t y )
{
	switch(pTexture->ePixelFormat) {
		case FORMAT_PIXEL_5650:
			return Cat_ColorConvert5650To8888( Cat_TextureGetPixel16( pTexture, x, y ) );
		case FORMAT_PIXEL_5551:
			return Cat_ColorConvert5551To8888( Cat_TextureGetPixel16( pTexture, x, y ) );
		case FORMAT_PIXEL_4444:
			return Cat_ColorConvert4444To8888( Cat_TextureGetPixel16( pTexture, x, y ) );
		case FORMAT_PIXEL_8888:
			return Cat_TextureGetPixel32( pTexture, x, y );
		case FORMAT_PIXEL_CLUT8:
			return Cat_PaletteGetColor( pTexture->pPalette, Cat_TextureGetPixel8( pTexture, x, y ) );
		case FORMAT_PIXEL_CLUT4:
			if(pTexture->pPalette4) {
				return Cat_PaletteGetColor( pTexture->pPalette, pTexture->tbl4to8[Cat_TextureGetPixel4( pTexture, x, y )] );
			} else {
				return Cat_PaletteGetColor( pTexture->pPalette, Cat_TextureGetPixel4( pTexture, x, y ) );
			}
		default:
			return 0;
	}
}

//! テクスチャからテクセルを取得する
/*!
	@param[in]	pTexture	テクスチャ
	@param[in]	x			x座標
	@param[in]	y			y座標
	@return	ピクセル値
*/
uint32_t
Cat_TextureGetPixelRaw( Cat_Texture* pTexture, uint32_t x, uint32_t y )
{
	switch(pTexture->ePixelFormat) {
		case FORMAT_PIXEL_5650:
			return Cat_TextureGetPixel16( pTexture, x, y );
		case FORMAT_PIXEL_5551:
			return Cat_TextureGetPixel16( pTexture, x, y );
		case FORMAT_PIXEL_4444:
			return Cat_TextureGetPixel16( pTexture, x, y );
		case FORMAT_PIXEL_8888:
			return Cat_TextureGetPixel32( pTexture, x, y );
		case FORMAT_PIXEL_CLUT8:
			return Cat_TextureGetPixel8( pTexture, x, y );
		case FORMAT_PIXEL_CLUT4:
			if(pTexture->pPalette4) {
				return pTexture->tbl4to8[Cat_TextureGetPixel4( pTexture, x, y )];
			} else {
				return Cat_TextureGetPixel4( pTexture, x, y );
			}
		default:
			return 0;
	}
}

//! RGBA4444からRGBA8888へ変換する
uint32_t
Cat_ColorConvert4444To8888( uint16_t rgba4444 )
{
	uint32_t r = ( (uint32_t)rgba4444        & 0xF) << 4;
	uint32_t g = (((uint32_t)rgba4444 >>  4) & 0xF) << (4+8);
	uint32_t b = (((uint32_t)rgba4444 >>  8) & 0xF) << (4+16);
	uint32_t a = (((uint32_t)rgba4444 >> 12) & 0xF) << (4+24);

	if(r) r |= 0xF;
	if(g) g |= 0xF00;
	if(b) b |= 0xF0000;
	if(a) a |= 0xF000000;

	return r | g | b | a;
}

//! RGBA5650からRGBA8888へ変換する
uint32_t
Cat_ColorConvert5650To8888( uint16_t rgba5650 )
{
	uint32_t r = ( (uint32_t)rgba5650        & 0x1F) << 3;
	uint32_t g = (((uint32_t)rgba5650 >>  5) & 0x3F) << (2+8);
	uint32_t b = (((uint32_t)rgba5650 >> 11) & 0x1F) << (3+16);

	if(r) r |= 0x7;
	if(g) g |= 0x300;
	if(b) b |= 0x70000;

	return r | g | b | 0xFF000000;
}

//! RGBA5551からRGBA8888へ変換する
uint32_t
Cat_ColorConvert5551To8888( uint16_t rgba5551 )
{
	uint32_t r = ( (uint32_t)rgba5551        & 0x1F) << 3;
	uint32_t g = (((uint32_t)rgba5551 >>  5) & 0x1F) << (3+8);
	uint32_t b = (((uint32_t)rgba5551 >> 11) & 0x1F) << (3+16);
	uint32_t a = (((uint32_t)rgba5551 >> 15) & 0x01) << (7+24);

	if(r) r |= 0x7;
	if(g) g |= 0x700;
	if(b) b |= 0x70000;
	if(a) a |= 0x7F000000;

	return r | g | b | a;
}
