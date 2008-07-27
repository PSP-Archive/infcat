//! @file Cat_SJISConvert.c
// Shift-JISとUTFの変換を行う

#include "Cat_SJISConvert.h"
#include "Cat_UTFConvert.h"
#include "codetable/Cat_TABLE_SJIStoUTF16.h"
#include "codetable/Cat_TABLE_UTF16toSJIS.h"
#include <malloc.h>	// for memalign

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! UTF16からSJISへの変換時、変換できなかった文字に使う文字コード(SJIS) \n □ White Square
#define SJIS_UNKNOWN_CODE	(0x81a0)

//! SJISからUTF16への変換時、変換できなかった文字に使う文字コード(UTF-16) \n □ White Square
#define UTF16_UNKNOWN_CODE	(0x25a1)

//! 多バイト文字の１バイト目かどうかの判定
/*!
	@param[in]	nSJISCode	ShiftJISコード
	@return	多バイト文字の１バイト目だったら0以外を、そうでなかったら0を返す。
*/
static int
IsLeadByte( uint16_t nSJISCode )
{
	return ((0x81 <= nSJISCode) && (nSJISCode <= 0x9f))
		|| ((0xE0 <= nSJISCode) && (nSJISCode <= 0xFC));
}

//! 文字コードを変換する
/*!
	@param[in]	nCode			文字コード
	@param[in]	nUnknownCode	変換できなかったときに返す文字コード
	@param[in]	tblCode			文字変換テーブル
	@param[in]	nTableSize		文字変換テーブルのサイズ
	@return	変換された文字コード
	@todo	高速化（バイナリサーチ、ハッシュとか）
*/
static uint16_t
CodeSearch( uint16_t nCode, uint16_t nUnknownCode, const uint16_t* tblCode, size_t nTableSize )
{
	size_t i;
	for(i = 0; i < nTableSize; i += 2) {
		if(tblCode[i] == nCode) {
			return tblCode[i + 1];
		}
	}
	return nUnknownCode;
}

//! Shift-JISからUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pSJIS			変換する文字列(Shift-JIS)
	@param[out]	pUTF16			変換された文字列(UTF-16) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF16に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_SJIStoUTF16( const char* pSJIS, uint16_t* pUTF16, size_t nBufferLength )
{
	uint16_t nCode;
	const uint8_t* p = (const uint8_t*)pSJIS;
	size_t nLength;

	if(p == 0) {
		return 0;
	}

	nLength = sizeof(uint16_t);
	if(pUTF16) {
		if(nBufferLength < sizeof(uint16_t)) {
			return 0;
		}
		while((nCode = (uint16_t)*p++) && (nLength < nBufferLength)) {
			if(IsLeadByte( nCode )) {
				// 多バイト文字
				nCode <<= 8;
				nCode |= *p++;
			}
			*pUTF16++ = CodeSearch( nCode, UTF16_UNKNOWN_CODE, tblSJIStoUTF16, sizeof(tblSJIStoUTF16) / sizeof(tblSJIStoUTF16[0]) );
			nLength += sizeof(uint16_t);
		}
		*pUTF16 = 0;
	} else {
		while((nCode = (uint16_t)*p++)) {
			if(IsLeadByte( nCode )) {
				p++;
			}
			nLength += sizeof(uint16_t);
		}
	}
	return nLength;
}

//! UTF-16からShift-JISへ変換する
/*!
	\a pSJIS に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF16			変換する文字列(UTF-16)
	@param[out]	pSJIS			変換された文字列(Shift-JIS) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pSJISに0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF16toSJIS( const uint16_t* pUTF16, char* pSJIS, size_t nBufferLength )
{
	uint16_t nCode;
	size_t nLength;

	if(pUTF16 == 0) {
		return 0;
	}

	nLength = sizeof(char);
	if(pSJIS) {
		if(nBufferLength < sizeof(char)) {
			return 0;
		}
		while((nCode = *pUTF16++) && (nLength < nBufferLength)) {
			if((nCode & 0xFC00) == 0xD800) {
				// サロゲートペア
				if((*pUTF16 & 0xFC00) == 0xDC00) {
					pUTF16++;
				}
				nCode = SJIS_UNKNOWN_CODE;
			} else {
				nCode = CodeSearch( nCode, SJIS_UNKNOWN_CODE, tblUTF16toSJIS, sizeof(tblUTF16toSJIS) / sizeof(tblUTF16toSJIS[0]) );
			}
			if(IsLeadByte( nCode >> 8 )) {
				// 多バイト文字
				nLength += sizeof(char) * 2;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pSJIS++ = (char)(nCode >> 8);
				*pSJIS++ = (char)(nCode & 0x00FF);
			} else {
				nLength += sizeof(char);
				*pSJIS++ = (char)nCode;
			}
		}
		*pSJIS = 0;
	} else {
		while((nCode = *pUTF16++)) {
			if((nCode & 0xFC00) == 0xD800) {
				// サロゲートペア
				if((*pUTF16 & 0xFC00) == 0xDC00) {
					pUTF16++;
				}
				nCode = SJIS_UNKNOWN_CODE;
			} else {
				nCode = CodeSearch( nCode, SJIS_UNKNOWN_CODE, tblUTF16toSJIS, sizeof(tblUTF16toSJIS) / sizeof(tblUTF16toSJIS[0]) );
			}
			if(IsLeadByte( nCode >> 8 )) {
				// 多バイト文字
				nLength += sizeof(char) * 2;
			} else {
				nLength += sizeof(char);
			}
		}
	}
	return nLength;
}

//! Shift-JISからUTF-8へ変換する
/*!
	\a pSJIS に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pSJIS			変換する文字列(Shift-JIS)
	@param[out]	pUTF8			変換された文字列(UTF-8) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF8に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_SJIStoUTF8( const char* pSJIS, uint8_t* pUTF8, size_t nBufferLength )
{
	size_t nUTF8Length;
	size_t nUTF16Length;
	uint16_t* pTmp;

	if(pSJIS == 0) {
		return 0;
	}
	nUTF16Length = Cat_SJIStoUTF16( pSJIS, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (uint16_t*)CAT_MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_SJIStoUTF16( pSJIS, pTmp, nUTF16Length );
	nUTF8Length = Cat_UTF16toUTF8( pTmp, pUTF8, nBufferLength );
	CAT_FREE( pTmp );
	return nUTF8Length;
}

//! Shift-JISからUTF-32へ変換する
/*!
	\a pSJIS に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pSJIS			変換する文字列(Shift-JIS)
	@param[out]	pUTF32			変換された文字列(UTF-32) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF32に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_SJIStoUTF32( const char* pSJIS, uint32_t* pUTF32, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nUTF32Length;
	uint16_t* pTmp;

	if(pSJIS == 0) {
		return 0;
	}
	nUTF16Length = Cat_SJIStoUTF16( pSJIS, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (uint16_t*)CAT_MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_SJIStoUTF16( pSJIS, pTmp, nUTF16Length );
	nUTF32Length = Cat_UTF16toUTF32( pTmp, pUTF32, nBufferLength );
	CAT_FREE( pTmp );
	return nUTF32Length;
}

//! UTF-8からShift-JISへ変換する
/*!
	\a pSJIS に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF8			変換する文字列(UTF-8)
	@param[out]	pSJIS			変換された文字列(Shift-JIS) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pSJISに0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF8toSJIS( const uint8_t* pUTF8, char* pSJIS, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nSJISLength;
	uint16_t* pTmp;

	if(pUTF8 == 0) {
		return 0;
	}
	nUTF16Length = Cat_UTF8toUTF16( pUTF8, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (unsigned short*)CAT_MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF8toUTF16( pUTF8, pTmp, nUTF16Length );
	nSJISLength = Cat_UTF16toSJIS( pTmp, pSJIS, nBufferLength );
	CAT_FREE( pTmp );
	return nSJISLength;
}

//! UTF-32からShift-JISへ変換する
/*!
	\a pSJIS に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF32	変換する文字列(UTF-32)
	@param[out]	pSJIS	変換された文字列(Shift-JIS) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pSJISに0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF32toSJIS( const uint32_t* pUTF32, char* pSJIS, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nSJISLength;
	uint16_t* pTmp;

	if(pUTF32 == 0) {
		return 0;
	}

	nUTF16Length = Cat_UTF32toUTF16( pUTF32, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (unsigned short*)CAT_MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF32toUTF16( pUTF32, pTmp, nUTF16Length );
	nSJISLength = Cat_UTF16toSJIS( pTmp, pSJIS, nBufferLength );
	CAT_FREE( pTmp );
	return nSJISLength;
}
