//! @file	Cat_UTFConvert.c
// UTF-8,UTF-16,UTF-32の変換をする

#include <stdlib.h>
#include "Cat_UTFConvert.h"

//! メモリ確保マクロ
#define MALLOC(x) malloc(x)
//! メモリ解放マクロ
#define FREE(x) free(x)

//! UTF-16からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF16	変換する文字列(UTF-16)
	@param[out]	pUTF32	変換された文字列(UTF-32) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_UTF16toUTF32( const unsigned short* pUTF16, unsigned int* pUTF32 )
{
	int nLength;

	if(pUTF16 == 0) {
		return 0;
	}

	nLength = 4;

	while(*pUTF16) {
		if((*pUTF16 & 0xFC00) == 0xD800) {
			if((pUTF16[1] & 0xFC00) == 0xDC00) {
				if(pUTF32) {
					// サロゲートペアの処理
					// U+10000 ～ U+10FFFF
					unsigned int nCode;
					nCode = (((unsigned int)pUTF16[0] & 0x03FF) << 10)
								| ((unsigned int)pUTF16[1] & 0x03FF);
					*pUTF32++ = nCode + 0x10000;
					pUTF16++;
				}
			}
		} else {
			if(pUTF32) {
				*pUTF32++ = *pUTF16;
			}
		}
		pUTF16++;
		nLength += 4;
	}

	if(pUTF32) {
		*pUTF32 = 0;
	}
	return nLength;
}

//! UTF-32からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF32	変換する文字列(UTF-32)
	@param[out]	pUTF16	変換された文字列(UTF-16) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_UTF32toUTF16( const unsigned int* pUTF32, unsigned short* pUTF16 )
{
	int nLength;
	const unsigned int* pSrc;

	if(pUTF32 == 0) {
		return 0;
	}

	nLength = 2;
	pSrc = pUTF32;

	while(*pSrc) {
		if(*pSrc <= 0x0000FFFF) {
			nLength += 2;
			if(pUTF16) {
				*pUTF16++ = (unsigned short)*pSrc;
			}
		} else if(*pSrc <= 0x0010FFFF) {
			// 16bitでは、U+10000 ～ U+10FFFFを表現できないので、
			// サロゲートペアを使って表現する。
			nLength += 4;
			if(pUTF16) {
				// U+10000 ～ U+10FFFF
				unsigned int nCode = *pSrc - 0x10000;
				*pUTF16++ = ((nCode >> 10) & 0x3FF) | 0xD800;
				*pUTF16++ = ( nCode        & 0x3FF) | 0xDC00;
			}
		}
		pSrc++;
	}

	if(pUTF16) {
		*pUTF16 = 0;
	}
	return nLength;
}

//! UTF-8からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF8	変換する文字列(UTF-8)
	@param[out]	pUTF32	変換された文字列(UTF-32) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_UTF8toUTF32( const unsigned char* pUTF8, unsigned int* pUTF32 )
{
	int nLength;
	const unsigned char* pSrc = pUTF8;

	if(pUTF8 == 0) {
		return 0;
	}

	nLength = 4;
	while(*pSrc) {
		if(*pSrc <= 0x7F) {
			if(pUTF32) {
				*pUTF32++ = *pSrc;
			}
			pSrc++;
		} else {
			if((*pSrc & 0xE0) == 0xC0) {
				if(pUTF32) {
					unsigned int c;
					c = ((unsigned int)pSrc[0] & 0x1F) << 6;
					c |= ((unsigned int)pSrc[1] & 0x3F);
					*pUTF32++ = c;
				}
				pSrc += 2;
			} else if((*pSrc & 0xF0) == 0xE0) {
				if(pUTF32) {
					unsigned int c;
					c = ((unsigned int)pSrc[0] & 0x0F) << 12;
					c |= ((unsigned int)pSrc[1] & 0x3F) << 6;
					c |= ((unsigned int)pSrc[2] & 0x3F);
					*pUTF32++ = c;
				}
				pSrc += 3;
			} else if((*pSrc & 0xF8) == 0xF0) {
				if(pUTF32) {
					unsigned int c;
					c = ((unsigned int)pSrc[0] & 0x07) << 18;
					c |= ((unsigned int)pSrc[1] & 0x3F) << 12;
					c |= ((unsigned int)pSrc[2] & 0x3F) << 6;
					c |= ((unsigned int)pSrc[3] & 0x3F);
					*pUTF32++ = c;
				}
				pSrc += 4;
			} else if((*pSrc & 0xFC) == 0xF8) {
				if(pUTF32) {
					unsigned int c;
					c = ((unsigned int)pSrc[0] & 0x03) << 24;
					c |= ((unsigned int)pSrc[1] & 0x3F) << 18;
					c |= ((unsigned int)pSrc[2] & 0x3F) << 12;
					c |= ((unsigned int)pSrc[3] & 0x3F) << 6;
					c |= ((unsigned int)pSrc[4] & 0x3F);
					*pUTF32++ = c;
				}
				pSrc += 5;
			} else if((*pSrc & 0xFE) == 0xFC) {
				if(pUTF32) {
					unsigned int c;
					c = ((unsigned int)pSrc[0] & 0x01) << 30;
					c |= ((unsigned int)pSrc[1] & 0x3F) << 24;
					c |= ((unsigned int)pSrc[2] & 0x3F) << 18;
					c |= ((unsigned int)pSrc[3] & 0x3F) << 12;
					c |= ((unsigned int)pSrc[4] & 0x3F) << 6;
					c |= ((unsigned int)pSrc[5] & 0x3F);
					*pUTF32++ = c;
				}
				pSrc += 6;
			} else {
				/* なんか変 */
				pSrc++;
			}
		}
		nLength += 4;
	}

	if(pUTF32) {
		*pUTF32 = 0;
	}
	return nLength;
}

//! UTF-32からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF32	変換する文字列(UTF-32)
	@param[out]	pUTF8	変換された文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_UTF32toUTF8( const unsigned int* pUTF32, unsigned char* pUTF8 )
{
	int nLength;
	const unsigned int* pSrc = pUTF32;

	if(pUTF32 == 0) {
		return 0;
	}

	nLength = 1;
	while(*pSrc) {
		if(*pSrc <= 0x0000007F) {
			nLength += 1;
			if(pUTF8) {
				*pUTF8++ = (unsigned char)*pSrc;
			}
		} else if(*pSrc <= 0x000007FF) {
			nLength += 2;
			if(pUTF8) {
				*pUTF8++ = (*pSrc >> 6)   | 0xC0;
				*pUTF8++ = (*pSrc & 0x3F) | 0x80;
			}
		} else if(*pSrc <= 0x0000FFFF) {
			nLength += 3;
			if(pUTF8) {
				*pUTF8++ =  (*pSrc >> 12)         | 0xE0;
				*pUTF8++ = ((*pSrc >>  6) & 0x3F) | 0x80;
				*pUTF8++ = ( *pSrc        & 0x3F) | 0x80;
			}
		} else if(*pSrc <= 0x001FFFFF) {
			nLength += 4;
			if(pUTF8) {
				*pUTF8++ =  (*pSrc >> 18)         | 0xF0;
				*pUTF8++ = ((*pSrc >> 12) & 0x3F) | 0x80;
				*pUTF8++ = ((*pSrc >>  6) & 0x3F) | 0x80;
				*pUTF8++ = ( *pSrc        & 0x3F) | 0x80;
			}
		} else if(*pSrc <= 0x03FFFFFF) {
			nLength += 5;
			if(pUTF8) {
				*pUTF8++ =  (*pSrc >> 24)         | 0xF8;
				*pUTF8++ = ((*pSrc >> 18) & 0x3F) | 0x80;
				*pUTF8++ = ((*pSrc >> 12) & 0x3F) | 0x80;
				*pUTF8++ = ((*pSrc >>  6) & 0x3F) | 0x80;
				*pUTF8++ = ( *pSrc        & 0x3F) | 0x80;
			}
		} else {
			nLength += 6;
			if(pUTF8) {
				*pUTF8++ =  (*pSrc >> 30)         | 0xFC;
				*pUTF8++ = ((*pSrc >> 24) & 0x3F) | 0x80;
				*pUTF8++ = ((*pSrc >> 18) & 0x3F) | 0x80;
				*pUTF8++ = ((*pSrc >> 12) & 0x3F) | 0x80;
				*pUTF8++ = ((*pSrc >>  6) & 0x3F) | 0x80;
				*pUTF8++ = ( *pSrc        & 0x3F) | 0x80;
			}
		}
		pSrc++;
	}

	if(pUTF8) {
		*pUTF8 = 0;
	}
	return nLength;
}

//! UTF-8からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF8	変換する文字列(UTF-8)
	@param[out]	pUTF16	変換された文字列(UTF-16) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_UTF8toUTF16( const unsigned char* pUTF8, unsigned short* pUTF16 )
{
	int rc;
	unsigned int* pTmp;

	if(pUTF8 == 0) {
		return 0;
	}

	pTmp = (unsigned int*)MALLOC( Cat_UTF8toUTF32( pUTF8, 0 ) );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF8toUTF32( pUTF8, pTmp );
	rc = Cat_UTF32toUTF16( pTmp, pUTF16 );
	FREE( pTmp );
	return rc;
}

//! UTF-16からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF16	変換する文字列(UTF-16)
	@param[out]	pUTF8	変換された文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_UTF16toUTF8( const unsigned short* pUTF16, unsigned char* pUTF8 )
{
	int rc;
	unsigned int* pTmp;

	if(pUTF16 == 0) {
		return 0;
	}

	pTmp = (unsigned int*)MALLOC( Cat_UTF16toUTF32( pUTF16, 0 ) );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF16toUTF32( pUTF16, pTmp );
	rc = Cat_UTF32toUTF8( pTmp, pUTF8 );
	FREE( pTmp );
	return rc;
}
