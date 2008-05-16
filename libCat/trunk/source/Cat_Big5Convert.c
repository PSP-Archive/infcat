//! @file Cat_Big5Convert.c
// 繁体字中国語 Big5（Traditional Chinese）とUTFの変換を行う

#include "Cat_Big5Convert.h"
#include "Cat_UTFConvert.h"
#include "codetable/Cat_TABLE_BIG5toUTF16.h"
#include "codetable/Cat_TABLE_UTF16toBIG5.h"

//! メモリ確保マクロ
#define MALLOC(x) malloc(x)
//! メモリ解放マクロ
#define FREE(x) free(x)

//! UTF16からBIG5への変換時、変換できなかった文字に使う文字コード(BIG5) \n □ White Square
#define BIG5_UNKNOWN_CODE	(0xa1bc)

//! BIG5からUTF16への変換時、変換できなかった文字に使う文字コード(UTF-16) \n □ White Square
#define UTF16_UNKNOWN_CODE	(0x25a1)

//! 多バイト文字の１バイト目かどうかの判定
/*!
	@param[in]	nBIG5Code	BIG5コード
	@return	多バイト文字の１バイト目だったら0以外を、そうでなかったら0を返す。
*/
static int
IsLeadByte( uint16_t nBIG5Code )
{
	return ((0xA1 <= nBIG5Code) && (nBIG5Code <= 0xC6))
		|| ((0xC9 <= nBIG5Code) && (nBIG5Code <= 0xF9));
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

//! Big5からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pBig5			変換する文字列(Big5)
	@param[out]	pUTF16			変換された文字列(UTF-16) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF16に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_Big5toUTF16( const char* pBig5, uint16_t* pUTF16, size_t nBufferLength )
{
	uint16_t nCode;
	const uint8_t* p = (const uint8_t*)pBig5;
	size_t nLength;

	if(p == 0) {
		return 0;
	}

	nLength = sizeof(uint16_t);
	if(pUTF16) {
		if(nBufferLength < sizeof(uint16_t)) {
			return 0;
		}
		while((nCode = *p++) && (nLength < nBufferLength)) {
			if(IsLeadByte( nCode )) {
				// 多バイト文字
				nCode <<= 8;
				nCode |= *p++;
			}
			*pUTF16++ = CodeSearch( nCode, UTF16_UNKNOWN_CODE, tblBIG5toUTF16, sizeof(tblBIG5toUTF16) / sizeof(tblBIG5toUTF16[0]) );
			nLength += sizeof(uint16_t);
		}
		*pUTF16 = 0;
	} else {
		while((nCode = *p++)) {
			if(IsLeadByte( nCode )) {
				// 多バイト文字
				p++;
			}
			nLength += sizeof(uint16_t);
		}
	}
	return nLength;
}

//! UTF-16からBig5へ変換する
/*!
	\a pBig5 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF16			変換する文字列(UTF-16)
	@param[out]	pBig5			変換された文字列(Big5) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pBig5に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF16toBig5( const uint16_t* pUTF16, char* pBig5, size_t nBufferLength )
{
	uint16_t nCode;
	size_t nLength;

	if(pUTF16 == 0) {
		return 0;
	}

	nLength = sizeof(char);
	if(pBig5) {
		if(nBufferLength < sizeof(char)) {
			return 0;
		}
		while((nCode = *pUTF16++) && (nLength < nBufferLength)) {
			if((nCode & 0xFC00) == 0xD800) {
				// サロゲートペア
				if((*pUTF16 & 0xFC00) == 0xDC00) {
					pUTF16++;
				}
				nCode = BIG5_UNKNOWN_CODE;
			} else {
				nCode = CodeSearch( nCode, BIG5_UNKNOWN_CODE, tblUTF16toBIG5, sizeof(tblUTF16toBIG5) / sizeof(tblUTF16toBIG5[0]) );
			}
			if(IsLeadByte( nCode >> 8 )) {
				// 多バイト文字
				nLength += sizeof(char) * 2;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pBig5++ = (char)(nCode >> 8);
				*pBig5++ = (char)(nCode & 0x00FF);
			} else {
				nLength += sizeof(char);
				*pBig5++ = (char)nCode;
			}
		}
		*pBig5 = 0;
	} else {
		while((nCode = *pUTF16++)) {
			if((nCode & 0xFC00) == 0xD800) {
				// サロゲートペア
				if((*pUTF16 & 0xFC00) == 0xDC00) {
					pUTF16++;
				}
				nCode = BIG5_UNKNOWN_CODE;
			} else {
				nCode = CodeSearch( nCode, BIG5_UNKNOWN_CODE, tblUTF16toBIG5, sizeof(tblUTF16toBIG5) / sizeof(tblUTF16toBIG5[0]) );
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

//! Big5からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pBig5			変換する文字列(Big5)
	@param[out]	pUTF8			変換された文字列(UTF-8) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF8に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_Big5toUTF8( const char* pBig5, uint8_t* pUTF8, size_t nBufferLength )
{
	size_t nUTF8Length;
	size_t nUTF16Length;
	uint16_t* pTmp;

	if(pBig5 == 0) {
		return 0;
	}
	nUTF16Length = Cat_Big5toUTF16( pBig5, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (uint16_t*)MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_Big5toUTF16( pBig5, pTmp, nUTF16Length );
	nUTF8Length = Cat_UTF16toUTF8( pTmp, pUTF8, nBufferLength );
	FREE( pTmp );
	return nUTF8Length;
}

//! Big5からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pBig5			変換する文字列(Big5)
	@param[out]	pUTF32			変換された文字列(UTF-32) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF32に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_Big5toUTF32( const char* pBig5, uint32_t* pUTF32, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nUTF32Length;
	uint16_t* pTmp;

	if(pBig5 == 0) {
		return 0;
	}
	nUTF16Length = Cat_Big5toUTF16( pBig5, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (uint16_t*)MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_Big5toUTF16( pBig5, pTmp, nUTF16Length );
	nUTF32Length = Cat_UTF16toUTF32( pTmp, pUTF32, nBufferLength );
	FREE( pTmp );
	return nUTF32Length;
}

//! UTF-8からBig5へ変換する
/*!
	\a pBig5 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF8			変換する文字列(UTF-8)
	@param[out]	pBig5			変換された文字列(Big5) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pBig5に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF8toBig5( const uint8_t* pUTF8, char* pBig5, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nBig5Length;
	uint16_t* pTmp;

	if(pBig5 == 0) {
		return 0;
	}
	nUTF16Length = Cat_UTF8toUTF16( pUTF8, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (unsigned short*)MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF8toUTF16( pUTF8, pTmp, nUTF16Length );
	nBig5Length = Cat_UTF16toBig5( pTmp, pBig5, nBufferLength );
	FREE( pTmp );
	return nBig5Length;
}

//! UTF-32からBig5へ変換する
/*!
	\a pBig5 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF32	変換する文字列(UTF-32)
	@param[out]	pBig5	変換された文字列(Big5) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pBig5に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF32toBig5( const uint32_t* pUTF32, char* pBig5, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nBig5Length;
	uint16_t* pTmp;

	if(pUTF32 == 0) {
		return 0;
	}

	nUTF16Length = Cat_UTF32toUTF16( pUTF32, 0, 0 );
	if(nUTF16Length == 0) {
		return 0;
	}
	pTmp = (unsigned short*)MALLOC( nUTF16Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF32toUTF16( pUTF32, pTmp, nUTF16Length );
	nBig5Length = Cat_UTF16toBig5( pTmp, pBig5, nBufferLength );
	FREE( pTmp );
	return nBig5Length;
}
