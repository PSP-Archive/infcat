//! @file	Cat_Palette.h
// パレット関連

#ifndef INCL_Cat_Palette_h
#define INCL_Cat_Palette_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! パレットフォーマット
typedef enum {
	FORMAT_PALETTE_5650 = 0,			/*!< 16bit RGBA5650	*/
	FORMAT_PALETTE_5551 = 1,			/*!< 16bit RGBA5551	*/
	FORMAT_PALETTE_4444 = 2,			/*!< 16bit RGBA4444	*/
	FORMAT_PALETTE_8888 = 3,			/*!< 32bit RGBA8888	*/

	FORMAT_PALETTE_MAX					/*!< 最大値			*/
} FORMAT_PALETTE;

//! パレット構造体
typedef struct {
	FORMAT_PALETTE	ePaletteFormat;			/*!< パレットフォーマット			*/
	uint32_t	nSize;						/*!< パレットサイズ(32バイト単位)	*/
	uint32_t	nMask;						/*!< パレットインデックスのマスク値	*/
	uint32_t	nRef;						/*!< 参照カウンタ					*/
	void*		pvData;						/*!< パレットデータ					*/
} Cat_Palette;

//! パレットを作成する
/*!
	@param[in]	ePaletteFormat	パレットフォーマット(FORMAT_PALETTE_xxx)
	@param[in]	nSize			パレット数(16または256)
	@param[in]	pvColorMap		初期パレットデータ。0の場合は、初期化されない。
	@return 作成されたパレット。失敗した場合は0が返る。
*/
extern Cat_Palette* Cat_PaletteCreate( FORMAT_PALETTE ePaletteFormat, uint32_t nSize, const void* pvColorMap );

//! パレットを解放する
/*!
	@param[in]	pPalette	解放するパレット
*/
extern void Cat_PaletteRelease( Cat_Palette* pPalette );

//! パレットを複製する
/*!
	@param[in]	pPalette	解放するパレット
	@return 複製されたパレット。失敗した場合は0が返る。
*/
extern Cat_Palette* Cat_PaletteDuplicate( Cat_Palette* pPalette );

//! 参照カウンタを加算する
/*!
	@param[in]	pPalette	パレット
*/
extern void Cat_PaletteAddRef( Cat_Palette* pPalette );

//! パレットを設定する
/*!
	@param[in]	pPalette	設定するパレット
*/
extern void Cat_PaletteSetPalette( Cat_Palette* pPalette );

//! パレットの色を取得する
/*!
	@param[in]	pPalette	パレット
	@param[in]	nIndex		取得する色のインデックス
	@return RGBA8888形式の色
*/
extern uint32_t Cat_PaletteGetColor( const Cat_Palette* pPalette, uint32_t nIndex );

//! パレットの色を取得する
/*!
	@param[in]	pPalette	パレット
	@param[in]	nIndex		取得する色のインデックス
	@return 色
*/
extern uint32_t Cat_PaletteGetColorRaw( const Cat_Palette* pPalette, uint32_t nIndex );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_Palette_h
