//! @file Cat_EUCJPConvert.c
// EUC-JPとUTFの変換を行う

#include "Cat_EUCJPConvert.h"
#include "Cat_UTFConvert.h"
#include "codetable/Cat_TABLE_EUCJPtoUTF16.h"

//! メモリ確保マクロ
#define MALLOC(x) malloc(x)
//! メモリ解放マクロ
#define FREE(x) free(x)

//! UTF16からEUC-JPへの変換時、変換できなかった文字に使う文字コード(EUC-JP) \n □ White Square
#define EUC_JP_UNKNOWN_CODE	(0xA2A2)

//! EUC-JPからUTF16への変換時、変換できなかった文字に使う文字コード(UTF-16) \n □ White Square
#define UTF16_UNKNOWN_CODE	(0x25a1)

//! 多バイト文字の１バイト目かどうかの判定
/*!
	@param[in]	nEUCCode	EUCコード
	@return	多バイト文字の１バイト目だったら0以外を、そうでなかったら0を返す。
*/
static int
IsLeadByte( uint16_t nEUCCode )
{
	return ((0xA1 <= nEUCCode) && (nEUCCode <= 0xFE))
		|| ((0x8E <= nEUCCode) && (nEUCCode <= 0x8F));
}

//! ３バイト文字の１バイト目かどうかの判定
/*!
	@param[in]	nEUCCode	EUCコード
	@return	３バイト文字の１バイト目だったら0以外を、そうでなかったら0を返す。
*/
static int
IsLead3Bytes( uint16_t nEUCCode )
{
	return 0x8F == nEUCCode;
}

//! 文字コードを変換する
/*!
	@param[in]	nCode			文字コード
	@param[in]	nUnknownCode	変換できなかったときに返す文字コード
	@param[in]	tblCode1		文字変換テーブル
	@param[in]	nTableSize1		文字変換テーブルのサイズ
	@param[in]	tblCode2		文字変換テーブル
	@param[in]	nTableSize2		文字変換テーブルのサイズ
	@return	変換された文字コード
	@todo	高速化（バイナリサーチ、ハッシュとか）
*/
static uint32_t
CodeSearch( uint16_t nCode, uint16_t nUnknownCode, const uint16_t* tblCode1, size_t nTableSize1, const uint32_t* tblCode2, size_t nTableSize2 )
{
	size_t i;
	for(i = 0; i < nTableSize1; i += 2) {
		if(tblCode1[i] == nCode) {
			return (uint32_t)tblCode1[i + 1];
		}
	}

	for(i = 0; i < nTableSize2; i += 2) {
		if(tblCode2[i] == (uint32_t)nCode) {
			return tblCode2[i + 1];
		}
	}
	return (uint32_t)nUnknownCode;
}

//! EUC-JPからUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pEUCJP			変換する文字列(EUC-JP)
	@param[out]	pUTF16			変換された文字列(UTF-16) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF16に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_EUCJPtoUTF16( const char* pEUCJP, uint16_t* pUTF16, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nUTF32Length;
	uint32_t* pTmp;

	if(pEUCJP == 0) {
		return 0;
	}
	nUTF32Length = Cat_EUCJPtoUTF32( pEUCJP, 0, 0 );
	if(nUTF32Length == 0) {
		return 0;
	}
	pTmp = (uint32_t*)MALLOC( nUTF32Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_EUCJPtoUTF32( pEUCJP, pTmp, nUTF32Length );
	nUTF16Length = Cat_UTF32toUTF16( pTmp, pUTF16, nBufferLength );
	FREE( pTmp );
	return nUTF16Length;
}

//! EUC-JPからUTF-8へ変換する
/*!
	\a pEUCJP に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pEUCJP			変換する文字列(EUC-JP)
	@param[out]	pUTF8			変換された文字列(UTF-8) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF8に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_EUCJPtoUTF8( const char* pEUCJP, uint8_t* pUTF8, size_t nBufferLength )
{
	size_t nUTF8Length;
	size_t nUTF32Length;
	uint32_t* pTmp;

	if(pEUCJP == 0) {
		return 0;
	}
	nUTF32Length = Cat_EUCJPtoUTF32( pEUCJP, 0, 0 );
	if(nUTF32Length == 0) {
		return 0;
	}
	pTmp = (uint32_t*)MALLOC( nUTF32Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_EUCJPtoUTF32( pEUCJP, pTmp, nUTF32Length );
	nUTF8Length = Cat_UTF32toUTF8( pTmp, pUTF8, nBufferLength );
	FREE( pTmp );
	return nUTF8Length;
}

//! EUC-JPからUTF-32へ変換する
/*!
	\a pEUCJP に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pEUCJP			変換する文字列(EUC-JP)
	@param[out]	pUTF32			変換された文字列(UTF-32) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF32に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_EUCJPtoUTF32( const char* pEUCJP, uint32_t* pUTF32, size_t nBufferLength )
{
	uint16_t nCode;
	const uint8_t* p = (const uint8_t*)pEUCJP;
	int nLength;

	if(p == 0) {
		return 0;
	}

	nLength = sizeof(uint32_t);
	if(pUTF32) {
		if(nBufferLength < sizeof(uint32_t)) {
			return 0;
		}
		while((nCode = *p++) && (nLength < nBufferLength)) {
			if(IsLeadByte( nCode )) {
				if(!IsLead3Bytes( nCode )) {
					// 2バイト文字
					nCode <<= 8;
					nCode |= *p++;
					*pUTF32++ = CodeSearch( nCode, UTF16_UNKNOWN_CODE,
						tblEUCJPtoUTF16,       sizeof(tblEUCJPtoUTF16) / sizeof(tblEUCJPtoUTF16[0]),
						tblEUCJPtoUTF16_4byte, sizeof(tblEUCJPtoUTF16_4byte) / sizeof(tblEUCJPtoUTF16_4byte[0]) );
				} else {
					// 3バイト文字
					nCode = *p++;
					nCode <<= 8;
					nCode |= *p++;
					*pUTF32++ = CodeSearch( nCode, UTF16_UNKNOWN_CODE,
						tblEUCJPtoUTF16_0x8F,       sizeof(tblEUCJPtoUTF16_0x8F) / sizeof(tblEUCJPtoUTF16_0x8F[0]),
						tblEUCJPtoUTF16_0x8F_4byte, sizeof(tblEUCJPtoUTF16_0x8F_4byte) / sizeof(tblEUCJPtoUTF16_0x8F_4byte[0]) );
				}
			} else {
				*pUTF32++ = CodeSearch( nCode, UTF16_UNKNOWN_CODE,
					tblEUCJPtoUTF16, sizeof(tblEUCJPtoUTF16) / sizeof(tblEUCJPtoUTF16[0]),
					0, 0 );
			}
			nLength += sizeof(uint32_t);
		}
		*pUTF32 = 0;
	} else {
		while((nCode = *p)) {
			if(IsLead3Bytes( nCode )) {
				p += 3;	// 3バイト文字
			} else if(IsLeadByte( nCode )) {
				p += 2;	// 2バイト文字
			} else {
				p += 1;	// 1バイト文字
			}
			nLength += sizeof(uint32_t);
		}
	}
	return nLength;
}
