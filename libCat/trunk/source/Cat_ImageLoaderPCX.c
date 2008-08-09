//! @file	Cat_ImageLoaderPCX.c
// PCX形式の読み込み

#ifdef USE_CAT_IMAGELOADER_PCX

#include <malloc.h>	// for memalign
#include <string.h>
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

//! PCXのフラグ
#define MAGIC_NUMBER	(0x0A)

//! PCXのファイルヘッダ
typedef struct {
	uint8_t		nFlag;				/*!< Zsoftのフラグ 0x0A = Zsoft PCX file							*/
	uint8_t		nVersion;			/*!< バージョン番号 \n
										0:PC Paintbrush 2.5 \n
										2:PC Paintbrush 2.8(パレットあり) \n
										3:PC Paintbrush 2.8(パレットなし) \n
										4:PC Paintbrush 2.8 for Windows \n
										5:PC Paintbrush 3.0以降 \n
									*/
	uint8_t		nEncoding;			/*!< エンコーディング(0x01:PCXランレングス)							*/
	uint8_t		nBitPerPixcel;		/*!< 各プレーンあたりのビット数 (1,2,4,8)							*/
	uint16_t	nMinX;				/*!< X最小値														*/
	uint16_t	nMinY;				/*!< Y最小値														*/
	uint16_t	nMaxX;				/*!< X最大値														*/
	uint16_t	nMaxY;				/*!< Y最大値														*/
	uint16_t	nDotPerInchWidth;	/*!< イメージ寸法 横												*/
	uint16_t	nDotPerInchHeight;	/*!< イメージ寸法 縦												*/
	uint8_t		nPalette[48];		/*!< ヘッダパレット													*/
	uint8_t		nReseved;			/*!< Zsoftに予約されている(常に0)									*/
	uint8_t		nPlaneCount;		/*!< プレーン数														*/
	uint16_t	nPitch;				/*!< １ラインに必要なバイト数(常に偶数)								*/
	uint16_t	nPaletteFormat;		/*!< ヘッダパレットの特性 \n
										1:カラー又は白黒 \n
										2:グレースケール
									*/
	uint16_t	nScreenWidth;		/*!< 画面の水平方向ピクセル数(アスペクトを考慮するときに便利っぽい)	*/
	uint16_t	nScreenHeight;		/*!< 画面の垂直方向ピクセル数(アスペクトを考慮するときに便利っぽい)	*/
	uint8_t		nPadding[54];		/*!< 128バイトにするための空き										*/
} __attribute__((packed)) PCXHeader;


//! ヘッダをチェックする
/*!
	@param[in]	pHeader	チェックするヘッダ
	@return	正常なら1 \n
			異常なら0を返す。
*/
static int32_t
Cat_ImageLoaderCheckHeader( const PCXHeader* pHeader )
{
	if(pHeader == 0) {
		return 0;	// 引数が変
	}
	if(pHeader->nFlag != MAGIC_NUMBER) {
		return 0;	// フラグが変
	}
	switch(pHeader->nVersion) {
		case 0:	// PC Paintbrush 2.5
		case 2:	// PC Paintbrush 2.8(パレットあり)
		case 3:	// PC Paintbrush 2.8(パレットなし)
		case 4:	// PC Paintbrush 2.8 for Windows
		case 5:	// PC Paintbrush 3.0以降
			break;
		default:
			return 0;	// バージョンが変
	}
	if(pHeader->nEncoding != 0x01) {
		return 0;	// エンコーディングが変
	}
	switch(pHeader->nBitPerPixcel) {
		case 1:
		case 2:
		case 4:
		case 8:
			break;
		default:
			return 0;	// ドット深度変
	}
	if((pHeader->nMinX > pHeader->nMaxX)
		|| (pHeader->nMinY > pHeader->nMaxY)) {
		return 0;	// 範囲が変
	}
	if(pHeader->nReseved != 0) {
		return 0;	// 常に0のはずなのに違う
	}
	return 1;
}

//! フォーマットのチェック
/*!
	@param[in]	pStream ストリーム
	@return	対応形式なら1 \n
			違うなら0を返す。
*/
int32_t
Cat_ImageLoaderCheckPCX( Cat_Stream* pStream )
{
	int32_t rc = 0;
	int64_t pos;
	PCXHeader header;

	if(pStream == 0) {
		return 0;
	}

	pos = Cat_StreamTell( pStream );
	if(pos >= 0) {
		if(Cat_StreamRead( pStream, &header, sizeof(PCXHeader) ) == sizeof(PCXHeader)) {
			rc = Cat_ImageLoaderCheckHeader( &header );
		}
		Cat_StreamSeek( pStream, pos );
	}
	return rc;
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
	PCXHeader header;
	uint32_t nWidth;
	uint32_t nHeight;
	uint8_t nData;
	uint32_t y;
	uint32_t x;
	uint8_t* pbImage;
	uint8_t* pbDest;
	uint32_t nPitch;
	Cat_Texture* rc = 0;

	if(pStream == 0) {
		return 0;
	}

	// ヘッダ読み込み
	if(Cat_StreamRead( pStream, &header, sizeof(PCXHeader) ) != sizeof(PCXHeader)) {
		return 0;
	}
	if(Cat_ImageLoaderCheckHeader( &header ) == 0) {
		return 0;
	}

	nWidth  = header.nMaxX - header.nMinX + 1;
	nHeight = header.nMaxY - header.nMinY + 1;

	if((header.nBitPerPixcel == 8) && (header.nPlaneCount == 3)) {
		// 24bit
		nPitch = (nWidth * 4 + 15) & ~15;	// 16バイトアライメントに
		pbImage = (uint8_t*)CAT_MALLOC( nPitch * nHeight );
		if(pbImage == 0) {
			return 0;	// メモリ確保失敗
		}
		memset( pbImage, 0xFF, nPitch * nHeight );

		y = 0;
		x = 0;
		pbDest = pbImage;
		while(y < nHeight) {
			if(Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData)) {
				CAT_FREE( pbImage );
				return 0;
			}
			if(nData >= 0xc0) {
				int64_t nLength = nData & 0x3f;
				int64_t i;
				if((nLength == 0) ||
					(Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData))) {
					CAT_FREE( pbImage );
					return 0;
				}
				for(i = 0; i < nLength; i++) {
					pbDest[(x % header.nPitch) * 4 + (x / header.nPitch)] = nData;
					x++;
					if(x >= header.nPitch * 3) {
						x = 0;
						y++;
						if(y >= nHeight) {
							break;
						}
						pbDest += nPitch;
					}
				}
			} else {
				pbDest[(x % header.nPitch) * 4 + (x / header.nPitch)] = nData;
				x++;
				if(x >= header.nPitch * 3) {
					x = 0;
					y++;
					pbDest += nPitch;
				}
			}
		}
		rc = Cat_TextureCreate( nWidth, nHeight, nPitch, pbImage, FORMAT_PIXEL_8888, 0 );
		CAT_FREE( pbImage );
	} else if((header.nBitPerPixcel == 8) && (header.nPlaneCount == 1)) {
		// 256色パレット
		int32_t i;
		Cat_Palette* pPalette;
		uint8_t pbColorMap[256*4];	// スタック注意

		nPitch = (nWidth + 15) & ~15;	// 16バイトアライメントに

		pbImage = (uint8_t*)CAT_MALLOC( nPitch * nHeight );
		if(pbImage == 0) {
			return 0;	// メモリ確保失敗
		}
		memset( pbImage, 0, nPitch * nHeight );

		y = 0;
		x = 0;
		pbDest = pbImage;
		while(y < nHeight) {
			if(Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData)) {
				CAT_FREE( pbImage );
				return 0;
			}
			if((nData & 0xc0) == 0xc0) {
				int64_t nLength = nData & 0x3f;
				int64_t i;
				if((nLength == 0)
					|| (Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData))) {
					CAT_FREE( pbImage );
					return 0;
				}
				for(i = 0; i < nLength; i++) {
					if(nPitch > x) {
						pbDest[x] = nData;
					}
					x++;
					if(x >= header.nPitch) {
						x = 0;
						y++;
						if(y >= nHeight) {
							break;
						}
						pbDest += nPitch;
					}
				}
			} else {
				if(nPitch > x) {
					pbDest[x] = nData;
				}
				x++;
				if(x >= header.nPitch) {
					x = 0;
					y++;
					pbDest += nPitch;
				}
			}
		}

		// パレット
		// (EOF - 768)の位置にあるはずだけど、スタートマークの12を探す。
		do {
			if(Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData)) {
				CAT_FREE( pbImage );
				return 0;
			}
		} while(nData != 12);
		for(i = 0; i < 256; i++) {
			if(Cat_StreamRead( pStream, &pbColorMap[i * 4], 3 ) != 3) {
				CAT_FREE( pbImage );
				return 0;
			}
			pbColorMap[i * 4 + 3] = 0xFF;
		}
		pPalette = Cat_PaletteCreate( FORMAT_PALETTE_8888, 256, pbColorMap );
		if(pPalette == 0) {
			CAT_FREE( pbImage );
			return 0;
		}
		rc = Cat_TextureCreate( nWidth, nHeight, nPitch, pbImage, FORMAT_PIXEL_CLUT8, pPalette );
		Cat_PaletteRelease( pPalette );
		CAT_FREE( pbImage );
	}

	return rc;
}

static int32_t
RunLengthWrite( Cat_Stream* pStream, void* pvData, int32_t nDataSize )
{
	uint8_t* pbData = (uint8_t*)pvData;

	// 何もかんがえずに、書き出す
	while(nDataSize > 0) {
		if(*pbData < 0xc0) {
			if(Cat_StreamWrite( pStream, pbData, 1 ) != 1) {
				return -1;
			}
			pbData++;
			nDataSize--;
		} else {
			uint8_t nLength = 0xc1;
			if(Cat_StreamWrite( pStream, &nLength, 1 ) != 1) {
				return -1;
			}
			if(Cat_StreamWrite( pStream, pbData, 1 ) != 1) {
				return -1;
			}
			pbData++;
			nDataSize--;
		}
	}
	return 0;
}

//! PCX形式を読み込んでテクスチャを作成する
/*!
	@param[in]	pStream		ストリーム
	@param[in]	pTexture	テクスチャ
	@return	成功した場合は、0 \n
			失敗した場合は、負数が返る
*/
int32_t
Cat_ImageLoaderSavePCX( Cat_Stream* pStream, Cat_Texture* pTexture )
{
	PCXHeader header;
	uint32_t nWidth;
	uint32_t nHeight;
	uint32_t nImageSize;

	if((pStream == 0) || (pTexture == 0)) {
		return -1;
	}

	memset( &header, 0, sizeof(header) );

	// ヘッダ設定
	header.nFlag         = MAGIC_NUMBER;
	header.nVersion      = 5;
	header.nEncoding     = 1;
	nWidth  = pTexture->nTextureWidth;
	nHeight = pTexture->nTextureHeight;
	switch(pTexture->ePixelFormat) {
		case FORMAT_PIXEL_CLUT4:
		case FORMAT_PIXEL_CLUT8:
			header.nBitPerPixcel = 8;
			header.nPlaneCount   = 1;
			break;
		case FORMAT_PIXEL_8888:
		case FORMAT_PIXEL_5650:
		case FORMAT_PIXEL_5551:
		default:
			header.nBitPerPixcel = 8;
			header.nPlaneCount   = 3;
			break;
	}
	header.nPitch = (nWidth + 1) & ~1;	// 偶数に
	header.nMinX = 0;
	header.nMaxX = nWidth - 1;
	header.nMinY = 0;
	header.nMaxY = nHeight - 1;
	header.nDotPerInchWidth  = 0x48;
	header.nDotPerInchHeight = 0x48;
	header.nPaletteFormat = 1;
	header.nScreenWidth  = 480;
	header.nScreenHeight = 272;

	// ヘッダ書き込み
	if(Cat_StreamWrite( pStream, &header, sizeof(header) ) != sizeof(header)) {
		return -1;
	}

	nImageSize = header.nPitch * nHeight * header.nPlaneCount;
	uint8_t* pbImage = (uint8_t*)CAT_MALLOC( nImageSize );
	memset( pbImage, 0, nImageSize );
	// イメージ書き込み
	switch(pTexture->ePixelFormat) {
		case FORMAT_PIXEL_CLUT4:
			{
				uint8_t* pbSrc = pbImage;
				uint32_t x;
				uint32_t y;
				uint32_t i;
				uint8_t nData;
				for(y = 0; y < nHeight; y++) {
					for(x = 0; x < nWidth; x++) {
						*pbSrc++ = Cat_TextureGetPixelRaw( pTexture, x, y );
					}
					if(nWidth & 1) {
						pbSrc++;
					}
				}
				if(RunLengthWrite( pStream, pbImage, nImageSize ) < 0) {
					CAT_FREE( pbImage );
					return -1;
				}

				// パレット
				nData = 12;
				if(Cat_StreamWrite( pStream, &nData, sizeof(nData) ) != sizeof(nData)) {
					CAT_FREE( pbImage );
					return -1;
				}
				if(pTexture->pPalette4 == 0) {
					for(i = 0; i < 16; i++) {
						uint32_t nColor = Cat_PaletteGetColor( pTexture->pPalette, i );
						if(Cat_StreamWrite( pStream, &nColor, 3 ) != 3) {
							CAT_FREE( pbImage );
							return -1;
						}
					}
				} else {
					for(i = 0; i < 16; i++) {
						uint32_t nColor = Cat_PaletteGetColor( pTexture->pPalette, pTexture->tbl4to8[i] );
						if(Cat_StreamWrite( pStream, &nColor, 3 ) != 3) {
							CAT_FREE( pbImage );
							return -1;
						}
					}
				}
				for(;i < 256; i++) {
					uint32_t nColor = 0;
					if(Cat_StreamWrite( pStream, &nColor, 3 ) != 3) {
						CAT_FREE( pbImage );
						return -1;
					}
				}
			}
			break;

		case FORMAT_PIXEL_CLUT8:
			{
				uint8_t* pbSrc = pbImage;
				uint32_t x;
				uint32_t y;
				uint32_t i;
				uint8_t nData;
				for(y = 0; y < nHeight; y++) {
					for(x = 0; x < nWidth; x++) {
						*pbSrc++ = Cat_TextureGetPixelRaw( pTexture, x, y );
					}
					if(nWidth & 1) {
						pbSrc++;
					}
				}
				if(RunLengthWrite( pStream, pbImage, nImageSize ) < 0) {
					CAT_FREE( pbImage );
					return -1;
				}

				// パレット
				nData = 12;
				if(Cat_StreamWrite( pStream, &nData, sizeof(nData) ) != sizeof(nData)) {
					CAT_FREE( pbImage );
					return -1;
				}
				for(i = 0; i < 256; i++) {
					uint32_t nColor = Cat_PaletteGetColor( pTexture->pPalette, i );
					if(Cat_StreamWrite( pStream, &nColor, 3 ) != 3) {
						CAT_FREE( pbImage );
						return -1;
					}
				}
			}
			break;
		case FORMAT_PIXEL_4444:
		case FORMAT_PIXEL_5551:
		case FORMAT_PIXEL_5650:
		case FORMAT_PIXEL_8888:
			{
				uint8_t* pbSrc = pbImage;
				uint32_t x;
				uint32_t y;
				for(y = 0; y < nHeight; y++) {
					for(x = 0; x < nWidth; x++) {
						uint32_t nColor = Cat_TextureGetPixel( pTexture, x, y );
						pbSrc[x                  ] = *((uint8_t*)&nColor + 0);
						pbSrc[x + header.nPitch*1] = *((uint8_t*)&nColor + 1);
						pbSrc[x + header.nPitch*2] = *((uint8_t*)&nColor + 2);
					}
					pbSrc += header.nPitch * 3;
				}
				if(RunLengthWrite( pStream, pbImage, nImageSize ) < 0) {
					CAT_FREE( pbImage );
					return -1;
				}
			}
			break;
		default:
			break;
	}
	CAT_FREE( pbImage );

	return 0;
}

#endif	// USE_CAT_IMAGELOADER_PCX
