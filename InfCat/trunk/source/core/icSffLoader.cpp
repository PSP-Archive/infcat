//! @file icSffLoader.cpp
// Sff形式の画像を読み込む

#include "icCore.h"

namespace ic {

enum enumPaletteType {
	ePaletteTypeINDIV = 0,
	ePaletteTypeSHARED = 1,
};

//! テクスチャを作成する
static Cat_Texture* SffCreateTexture( Cat_Stream* pStream );

#pragma pack(1)
struct SffFileHeader {
/*   0 */	uint8_t			m_cMAGIC[12];		/*!< マジックナンバー	*/
/*  12 */	uint32_t		m_dummy;			/*!< 不明 バージョン？	*/
/*  16 */	uint32_t		m_nCountGroup;		/*!< グループ数			*/
/*  20 */	uint32_t		m_nCountImage;		/*!< イメージ数			*/
/*  24 */	uint32_t		m_nImageOffset;		/*!< オフセット			*/
/*  28 */	uint32_t		m_nImageHeaderSize;	/*!< サイズ				*/
/*  32 */	uint32_t		m_nPaletteType; 	/*!< パレットタイプ		*/
/*  36 */	uint8_t			padding[512-36];
}; // 512バイト
#pragma pack()

#pragma pack(1)
struct SffImageHeader {
/*   0 */	uint32_t	m_nNextImageHeaderPosition;	/*!< 次のヘッダ位置									*/
/*   4 */	uint32_t	m_nImageSize;				/*!< PCXイメージサイズ								*/
/*   8 */	int16_t		m_nDrawOffsetX;				/*!< 表示オフセットX(ドット単位)					*/
/*  10 */	int16_t		m_nDrawOffsetY;				/*!< 表示オフセットY(ドット単位)					*/
/*  12 */	uint16_t	m_nGroupNo;					/*!< グループ番号									*/
/*  14 */	uint16_t	m_nItemNo;					/*!< グループ内番号									*/
/*  16 */	uint16_t	m_nLinkIndex;				/*!< 共有イメージ									*/
/*  18 */	uint16_t	m_fCommonPalette;			/*!< 共通パレット									*/
/*  20 */	uint16_t	d2[5];
/*  30 */	uint16_t	m_nPaletteInfo;				/*!< パレット情報 使ってない部分をワークとして	*/
}; // 32バイト
#pragma pack()

//! 識別用文字列
#define MAGIC_STRING "ElecbyteSpr"

//! ヘッダをチェックする
/*!
	@param[in]	header	Sffヘッダ
	@return	大丈夫ならtrue \n
			駄目ならfalseを返す
*/
static bool
CheckHeader( const SffFileHeader& header )
{
	return
		(memcmp( header.m_cMAGIC, MAGIC_STRING, 12 ) == 0)
		// && (header.m_nCountGroup != 0x0)
		&& (header.m_nCountImage != 0x0)
		&& (header.m_nImageOffset == 0x0200)
		&& (header.m_nImageHeaderSize == 32)
		&& ((header.m_nPaletteType == ePaletteTypeINDIV) || (header.m_nPaletteType == ePaletteTypeSHARED));
}

//! コンストラクタ
icTextureCreatorSff::icTextureCreatorSff()
{
	icTexturePool::RegisterCreator( this );
}

//! 対応している形式かどうかを調べる
/*!
	@return	対応している形式の場合true \n
			非対応な場合は、falseを返す
*/
bool
icTextureCreatorSff::Check( Cat_Stream* pStream )
{
	bool rc = false;
	int64_t nPos = Cat_StreamTell( pStream );
	if(nPos >= 0) {
		// ファイルヘッダ読み込み
		SffFileHeader header;
		if(Cat_StreamRead( pStream, &header, sizeof(header) ) == sizeof(header)) {
			// ヘッダチェック
			rc = CheckHeader( header );
		}
		Cat_StreamSeek( pStream, nPos );
	}
	return rc;
}

//! 作成する
/*!
	@param[in]	pTexturePool	テクスチャプール
	@param[in]	pStream			ストリーム
	@param[in]	eCreateFlag		作成フラグ
	@return 正常終了時 true \n
			失敗時 false
*/
bool
icTextureCreatorSff::Create( icTexturePool* pTexturePool, Cat_Stream* pStream, icTexturePool::enumCreateFlag eCreateFlag )
{
	if(pStream == 0) {
		return false;
	}

	// ファイルヘッダ読み込み
	SffFileHeader header;
	if(Cat_StreamRead( pStream, &header, sizeof(header) ) != sizeof(header)) {
		return false;
	}
	// ファイルヘッダチェック
	if(!CheckHeader( header )) {
		return false;
	}

	icTexturePool::Texture& texture = pTexturePool->GetTexture();
	texture.clear();
	texture.reserve( header.m_nCountImage );

	// イメージの読み込み処理
	std::vector<SffImageHeader>	pImageHeader( header.m_nCountImage );
	for(uint32_t i = 0; i < header.m_nCountImage; i++) {
		if(Cat_StreamRead( pStream, &pImageHeader[i], sizeof(SffImageHeader) ) != sizeof(SffImageHeader)) {
			return false;
		}
		pImageHeader[i].m_nPaletteInfo = 0;
		if(pImageHeader[i].m_fCommonPalette) {
			if((i > 0) && (pImageHeader[i - 1].m_nPaletteInfo == 2)) {
				pImageHeader[i].m_nPaletteInfo = 2;
			} else {
				pImageHeader[i].m_nPaletteInfo = 1;
			}
		}
		if(((pImageHeader[i].m_nGroupNo == 0) || (pImageHeader[i].m_nGroupNo == 9000)) && (pImageHeader[i].m_nItemNo == 0)) {
			if(pImageHeader[i].m_fCommonPalette) {
				for(uint32_t j = i; (j > 0) && (pImageHeader[j].m_nPaletteInfo == 1); j--) {
					pImageHeader[j].m_nPaletteInfo = 2;
					if(pImageHeader[j - 1].m_nPaletteInfo == 0) {
						pImageHeader[j - 1].m_nPaletteInfo = 2;
					}
				}
			} else {
				pImageHeader[i].m_nPaletteInfo = 2;
			}
		}

		// イメージ作成
		if(pImageHeader[i].m_nImageSize == 0) {
			// イメージサイズ0は、共通イメージ
			if((pImageHeader[i].m_nLinkIndex < i) && texture[pImageHeader[i].m_nLinkIndex]) {
				texture.push_back( new icTexture( texture[pImageHeader[i].m_nLinkIndex], pImageHeader[i].m_nGroupNo, pImageHeader[i].m_nItemNo, pImageHeader[i].m_nDrawOffsetX, pImageHeader[i].m_nDrawOffsetY ) );
			} else {
				texture.push_back( 0 );
			}
		} else {
			Cat_Texture* pTexture = SffCreateTexture( pStream );
			if(pTexture) {
				texture.push_back( new icTexture( pTexture, pImageHeader[i].m_nGroupNo, pImageHeader[i].m_nItemNo, pImageHeader[i].m_nDrawOffsetX, pImageHeader[i].m_nDrawOffsetY ) );
				Cat_TextureRelease( pTexture );
			} else {
				texture.push_back( 0 );
			}
		}

		if(pImageHeader[i].m_nNextImageHeaderPosition) {
			Cat_StreamSeek( pStream, pImageHeader[i].m_nNextImageHeaderPosition );
		} else {
			break;
		}
	}

	// パレット処理
	// 何かを参考にしたけど、出典を思い出せない……
	Cat_Palette* pPaletteD = 0;
	Cat_Palette* pPalette1 = 0;
	int32_t found_1st = 0;
	for(uint32_t i = 0; i < header.m_nCountImage; i++) {
		// パレット処理
		// 何かを参考にしたけど、出典を思い出せない……
		const int fLinux = 1;
		const int fAct = 0;
		const int fPal256 = 1;
		const uint32_t nInvertShared = 0;
		if(texture[i]) {
			if(!found_1st && !fAct && (fPal256 != 2)) {
				pPaletteD = texture[i]->GetPalette();
				pPalette1 = texture[i]->GetPalette();
				found_1st = 1;
			} else if((pImageHeader[i].m_nPaletteInfo == 2) || (fAct && !fLinux) || (fPal256 == 2)) {
				if(!(pImageHeader[i].m_nGroupNo == 9000 && pImageHeader[i].m_nItemNo == 1 && (!pImageHeader[i].m_fCommonPalette || fPal256 == -1)) || fPal256 == 2 ) {
					texture[i]->SetPalette( pPalette1 );
				}
			} else if((pImageHeader[i].m_nPaletteInfo == 1) && found_1st) {
				if((fPal256 == 1) || (fPal256 && !((pImageHeader[i].m_nGroupNo == 9000) && (pImageHeader[i].m_nItemNo == 1))) ) {
					texture[i]->SetPalette( pPaletteD );
				}
			} else if(fLinux) {
				if((fPal256 == 1) || !found_1st) {
					pPaletteD = texture[i]->GetPalette();
					found_1st = 1;
				}
			} else if(!fAct && (fPal256 == 1)) {
				if(pImageHeader[i].m_fCommonPalette) {
					texture[i]->SetPalette( pPaletteD );
				} else if(header.m_nPaletteType == nInvertShared) {
					pPaletteD = texture[i]->GetPalette();
				}
			}
		}
	}

	return true;
}

// 変則的な PCX 読み込み ---------------------------------------------------------------------------------------

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

//! テクスチャを作成する
static Cat_Texture*
SffCreateTexture( Cat_Stream* pStream )
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
				if((nLength == 0) || (Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData))) {
					CAT_FREE( pbImage );
					return 0;
				}
				for(i = 0; i < nLength; i++) {
					pbDest[(x % (uint32_t)header.nPitch) * 4 + (x / (uint32_t)header.nPitch)] = nData;
					x++;
					if(x >= ((uint32_t)header.nPitch * 3)) {
						x = 0;
						y++;
						if(y >= nHeight) {
							break;
						}
						pbDest += nPitch;
					}
				}
			} else {
				pbDest[(x % (uint32_t)header.nPitch) * 4 + (x / (uint32_t)header.nPitch)] = nData;
				x++;
				if(x >= ((uint32_t)header.nPitch * 3)) {
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
				if((nLength == 0) || (Cat_StreamRead( pStream, &nData, sizeof(nData) ) != sizeof(nData))) {
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
		memset( pbColorMap, 0xFF, 256*4 );
		if(Cat_StreamRead( pStream, &nData, sizeof(nData) ) == sizeof(nData)) {
			if(nData == 12) {
				for(i = 0; i < 256; i++) {
					if(Cat_StreamRead( pStream, &pbColorMap[i * 4], 3 ) != 3) {
						break;
					}
				}
			}
		}
		pPalette = Cat_PaletteCreate( FORMAT_PALETTE_8888, 256, pbColorMap );
		if(pPalette == 0) {
			CAT_FREE( pbImage );
			return 0;
		}
		rc = Cat_TextureCreate( nWidth, nHeight, nPitch, pbImage, FORMAT_PIXEL_CLUT8, pPalette );
		Cat_PaletteRelease( pPalette );
		CAT_FREE( pbImage );
		return rc;
	}
	return 0;
}

} // namespace ic
