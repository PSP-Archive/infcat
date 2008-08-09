//! @file	Cat_Palette.c
// パレット関連

#include "Cat_Palette.h"
#include "Cat_Texture.h"
#include <pspgu.h>
#include <malloc.h>	// for memalign
#include <string.h>

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! パレットを作成する
/*!
	@param[in]	ePaletteFormat	パレットフォーマット(FORMAT_PALETTE_xxx)
	@param[in]	nSize			パレット数(16または256)
	@param[in]	pvColorMap		初期パレットデータ。0の場合は、初期化されない。
	@return 作成されたパレット。失敗した場合は0が返る。
*/
Cat_Palette*
Cat_PaletteCreate( FORMAT_PALETTE ePaletteFormat, uint32_t nSize, const void* pvColorMap )
{
	Cat_Palette* rc;

	if(ePaletteFormat >= FORMAT_PALETTE_MAX) {
		return 0;
	}
	if((nSize != 16) && (nSize != 256)) {
		return 0;
	}

	rc = (Cat_Palette*)CAT_MALLOC( sizeof(Cat_Palette) );
	if(rc) {
		memset( rc, 0, sizeof(Cat_Palette) );
		rc->ePaletteFormat = ePaletteFormat;
		if(ePaletteFormat == FORMAT_PALETTE_8888) {
			rc->nSize = nSize / 8;
		} else {
			rc->nSize = nSize / 16;
		}
		if(nSize == 256) {
			rc->nMask = 0xFF;
		} else {
			rc->nMask = 0xF;
		}
		rc->pvData = (void*)CAT_MALLOC( rc->nSize * 32 );
		if(rc->pvData) {
			if(pvColorMap) {
				// 初期化
				memcpy( rc->pvData, pvColorMap, rc->nSize * 32 );
			}
			// 参照カウンタ
			rc->nRef = 1;
		} else {
			/* メモリ確保失敗 */
			CAT_FREE( rc );
			rc = 0;
		}
	}
	return rc;
}

//! パレットを解放する
/*!
	@param[in]	pPalette	解放するパレット
*/
void
Cat_PaletteRelease( Cat_Palette* pPalette )
{
	if(pPalette == 0) {
		return;
	}
	// 参照が無くなったら解放する
	if(--pPalette->nRef == 0) {
		if(pPalette->pvData) {
			CAT_FREE( pPalette->pvData );
		}
		memset( pPalette, 0, sizeof(Cat_Palette) );
		CAT_FREE( pPalette );
	}
}

//! パレットを複製する
/*!
	@param[in]	pPalette	解放するパレット
	@return 複製されたパレット。失敗した場合は0が返る。
*/
Cat_Palette*
Cat_PaletteDuplicate( Cat_Palette* pPalette )
{
	if(pPalette == 0) {
		return 0;
	}
	return Cat_PaletteCreate( pPalette->ePaletteFormat, pPalette->nMask + 1, pPalette->pvData );
}

//! パレットを設定する
/*!
	@param[in]	pPalette	設定するパレット
*/
void
Cat_PaletteSetPalette( Cat_Palette* pPalette )
{
	if(pPalette == 0) {
		return;
	}
	sceGuClutMode( (int)pPalette->ePaletteFormat, 0, pPalette->nMask, 0 );
	sceGuClutLoad( pPalette->nSize, pPalette->pvData );
}

//! パレットの色を取得する
/*!
	@param[in]	pPalette	パレット
	@param[in]	nIndex		取得する色のインデックス
	@return RGBA8888形式の色
*/
uint32_t
Cat_PaletteGetColor( const Cat_Palette* pPalette, uint32_t nIndex )
{
	if(pPalette == 0) {
		return 0;
	}
	if(nIndex > pPalette->nMask) {
		return 0;
	}
	switch(pPalette->ePaletteFormat) {
		case FORMAT_PALETTE_8888:
			return *((uint32_t*)pPalette->pvData + nIndex);
		case FORMAT_PALETTE_5650:
			return Cat_ColorConvert5650To8888( *((uint16_t*)pPalette->pvData + nIndex) );
		case FORMAT_PALETTE_5551:
			return Cat_ColorConvert5551To8888( *((uint16_t*)pPalette->pvData + nIndex) );
		case FORMAT_PALETTE_4444:
			return Cat_ColorConvert4444To8888( *((uint16_t*)pPalette->pvData + nIndex) );
		default:
			return 0;
	}
}

//! パレットの色を取得する
/*!
	@param[in]	pPalette	パレット
	@param[in]	nIndex		取得する色のインデックス
	@return 色
*/
uint32_t
Cat_PaletteGetColorRaw( const Cat_Palette* pPalette, uint32_t nIndex )
{
	if(pPalette == 0) {
		return 0;
	}
	if(nIndex > pPalette->nMask) {
		return 0;
	}
	switch(pPalette->ePaletteFormat) {
		case FORMAT_PALETTE_8888:
			return *((uint32_t*)pPalette->pvData + nIndex);
		case FORMAT_PALETTE_5650:
		case FORMAT_PALETTE_5551:
		case FORMAT_PALETTE_4444:
			return *((uint16_t*)pPalette->pvData + nIndex);
		default:
			return 0;
	}
}
