//! @file	Cat_UTFConvert.c
// UTF-8,UTF-16,UTF-32の変換をする

#include "Cat_UTFConvert.h"

//! メモリ確保マクロ
#define MALLOC(x) malloc(x)
//! メモリ解放マクロ
#define FREE(x) free(x)

//! UTF-16からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF16			変換する文字列(UTF-16)
	@param[out]	pUTF32			変換された文字列(UTF-32) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF32に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF16toUTF32( const uint16_t* pUTF16, uint32_t* pUTF32, size_t nBufferLength )
{
	size_t nLength;

	if(pUTF16 == 0) {
		return 0;
	}

	nLength = sizeof(uint32_t);
	if(pUTF32) {
		if(nBufferLength < sizeof(uint32_t)) {
			return 0;
		}
		while(*pUTF16 && (nLength < nBufferLength)) {
			if((*pUTF16 & 0xFC00) == 0xD800) {
				if((pUTF16[1] & 0xFC00) == 0xDC00) {
					// サロゲートペアの処理
					// U+10000 ～ U+10FFFF
					uint32_t nCode;
					nCode = (((uint32_t)pUTF16[0] & 0x03FF) << 10)
								| ((uint32_t)pUTF16[1] & 0x03FF);
					*pUTF32++ = nCode + 0x10000;
					pUTF16++;
					nLength += sizeof(uint32_t);
				}
			} else {
				*pUTF32++ = *pUTF16;
				nLength += sizeof(uint32_t);
			}
			pUTF16++;
		}
		*pUTF32 = 0;
	} else {
		while(*pUTF16) {
			if((*pUTF16 & 0xFC00) == 0xD800) {
				if((pUTF16[1] & 0xFC00) == 0xDC00) {
					pUTF16++;
					nLength += sizeof(uint32_t);
				}
			} else {
				nLength += sizeof(uint32_t);
			}
			pUTF16++;
		}
	}
	return nLength;
}

//! UTF-32からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF32			変換する文字列(UTF-32)
	@param[out]	pUTF16			変換された文字列(UTF-16) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF16に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF32toUTF16( const uint32_t* pUTF32, uint16_t* pUTF16, size_t nBufferLength )
{
	size_t nLength;

	if(pUTF32 == 0) {
		return 0;
	}

	nLength = sizeof(uint16_t);
	if(pUTF16) {
		if(nBufferLength < sizeof(uint16_t)) {
			return 0;
		}
		while(*pUTF32 && (nLength < nBufferLength)) {
			if(*pUTF32 <= 0x0000FFFF) {
				*pUTF16++ = (uint16_t)*pUTF32;
				nLength += sizeof(uint16_t);
			} else if(*pUTF32 <= 0x0010FFFF) {
				uint32_t nCode;
				if((nLength + sizeof(uint16_t)) >= nBufferLength) {
					break;	// バッファ足りない
				}
				// 16bitでは、U+10000 ～ U+10FFFFを表現できないので、
				// サロゲートペアを使って表現する。
				// U+10000 ～ U+10FFFF
				nCode = *pUTF32 - 0x10000;
				*pUTF16++ = ((nCode >> 10) & 0x3FF) | 0xD800;
				*pUTF16++ = ( nCode        & 0x3FF) | 0xDC00;
				nLength += sizeof(uint16_t) * 2;
			}
			pUTF32++;
		}
		*pUTF16 = 0;
	} else {
		while(*pUTF32) {
			if(*pUTF32 <= 0x0000FFFF) {
				nLength += sizeof(uint16_t);
			} else if(*pUTF32 <= 0x0010FFFF) {
				nLength += sizeof(uint16_t) * 2;
			}
			pUTF32++;
		}
	}
	return nLength;
}

//! UTF-8からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF8			変換する文字列(UTF-8)
	@param[out]	pUTF32			変換された文字列(UTF-32) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF32に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF8toUTF32( const uint8_t* pUTF8, uint32_t* pUTF32, size_t nBufferLength )
{
	size_t nLength;

	if(pUTF8 == 0) {
		return 0;
	}

	nLength = sizeof(uint32_t);
	if(pUTF32) {
		if(nBufferLength < sizeof(uint32_t)) {
			return 0;
		}
		while(*pUTF8 && (nLength < nBufferLength)) {
			if(*pUTF8 <= 0x7F) {
				*pUTF32++ = *pUTF8;
				nLength += sizeof(uint32_t);
				pUTF8++;
			} else {
				if((*pUTF8 & 0xE0) == 0xC0) {
					uint32_t nCode;
					nCode = ((uint32_t)pUTF8[0] & 0x1F) << 6;
					nCode |= ((uint32_t)pUTF8[1] & 0x3F);
					*pUTF32++ = nCode;
					nLength += sizeof(uint32_t);
					pUTF8 += 2;
				} else if((*pUTF8 & 0xF0) == 0xE0) {
					uint32_t nCode;
					nCode = ((uint32_t)pUTF8[0] & 0x0F) << 12;
					nCode |= ((uint32_t)pUTF8[1] & 0x3F) << 6;
					nCode |= ((uint32_t)pUTF8[2] & 0x3F);
					*pUTF32++ = nCode;
					nLength += sizeof(uint32_t);
					pUTF8 += 3;
				} else if((*pUTF8 & 0xF8) == 0xF0) {
					uint32_t nCode;
					nCode = ((uint32_t)pUTF8[0] & 0x07) << 18;
					nCode |= ((uint32_t)pUTF8[1] & 0x3F) << 12;
					nCode |= ((uint32_t)pUTF8[2] & 0x3F) << 6;
					nCode |= ((uint32_t)pUTF8[3] & 0x3F);
					*pUTF32++ = nCode;
					nLength += sizeof(uint32_t);
					pUTF8 += 4;
				} else if((*pUTF8 & 0xFC) == 0xF8) {
					uint32_t nCode;
					nCode = ((uint32_t)pUTF8[0] & 0x03) << 24;
					nCode |= ((uint32_t)pUTF8[1] & 0x3F) << 18;
					nCode |= ((uint32_t)pUTF8[2] & 0x3F) << 12;
					nCode |= ((uint32_t)pUTF8[3] & 0x3F) << 6;
					nCode |= ((uint32_t)pUTF8[4] & 0x3F);
					*pUTF32++ = nCode;
					nLength += sizeof(uint32_t);
					pUTF8 += 5;
				} else if((*pUTF8 & 0xFE) == 0xFC) {
					uint32_t nCode;
					nCode = ((uint32_t)pUTF8[0] & 0x01) << 30;
					nCode |= ((uint32_t)pUTF8[1] & 0x3F) << 24;
					nCode |= ((uint32_t)pUTF8[2] & 0x3F) << 18;
					nCode |= ((uint32_t)pUTF8[3] & 0x3F) << 12;
					nCode |= ((uint32_t)pUTF8[4] & 0x3F) << 6;
					nCode |= ((uint32_t)pUTF8[5] & 0x3F);
					*pUTF32++ = nCode;
					nLength += sizeof(uint32_t);
					pUTF8 += 6;
				} else {
					pUTF8++;	/* なんか変 */
				}
			}
		}
		*pUTF32 = 0;
	} else {
		while(*pUTF8) {
			if(*pUTF8 <= 0x7F) {
				nLength += sizeof(uint32_t);
				pUTF8++;
			} else {
				if((*pUTF8 & 0xE0) == 0xC0) {
					nLength += sizeof(uint32_t);
					pUTF8 += 2;
				} else if((*pUTF8 & 0xF0) == 0xE0) {
					nLength += sizeof(uint32_t);
					pUTF8 += 3;
				} else if((*pUTF8 & 0xF8) == 0xF0) {
					nLength += sizeof(uint32_t);
					pUTF8 += 4;
				} else if((*pUTF8 & 0xFC) == 0xF8) {
					nLength += sizeof(uint32_t);
					pUTF8 += 5;
				} else if((*pUTF8 & 0xFE) == 0xFC) {
					nLength += sizeof(uint32_t);
					pUTF8 += 6;
				} else {
					pUTF8++;	/* なんか変 */
				}
			}
		}
	}

	return nLength;
}

//! UTF-32からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF32			変換する文字列(UTF-32)
	@param[out]	pUTF8			変換された文字列(UTF-8) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF8に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF32toUTF8( const uint32_t* pUTF32, uint8_t* pUTF8, size_t nBufferLength )
{
	size_t nLength;

	if(pUTF32 == 0) {
		return 0;
	}

	nLength = sizeof(uint8_t);
	if(pUTF8) {
		if(nBufferLength < sizeof(uint8_t)) {
			return 0;
		}
		while(*pUTF32 && (nLength < nBufferLength)) {
			if(*pUTF32 <= 0x0000007F) {
				nLength += sizeof(uint8_t);
				*pUTF8++ = (uint8_t)*pUTF32;
			} else if(*pUTF32 <= 0x000007FF) {
				nLength += sizeof(uint8_t) * 2;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pUTF8++ = (uint8_t)((*pUTF32 >> 6)   | 0xC0);
				*pUTF8++ = (uint8_t)((*pUTF32 & 0x3F) | 0x80);
			} else if(*pUTF32 <= 0x0000FFFF) {
				nLength += sizeof(uint8_t) * 3;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pUTF8++ = (uint8_t)( (*pUTF32 >> 12)         | 0xE0);
				*pUTF8++ = (uint8_t)(((*pUTF32 >>  6) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(( *pUTF32        & 0x3F) | 0x80);
			} else if(*pUTF32 <= 0x001FFFFF) {
				nLength += sizeof(uint8_t) * 4;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pUTF8++ = (uint8_t)( (*pUTF32 >> 18)         | 0xF0);
				*pUTF8++ = (uint8_t)(((*pUTF32 >> 12) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(((*pUTF32 >>  6) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(( *pUTF32        & 0x3F) | 0x80);
			} else if(*pUTF32 <= 0x03FFFFFF) {
				nLength += sizeof(uint8_t) * 5;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pUTF8++ = (uint8_t)( (*pUTF32 >> 24)         | 0xF8);
				*pUTF8++ = (uint8_t)(((*pUTF32 >> 18) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(((*pUTF32 >> 12) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(((*pUTF32 >>  6) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(( *pUTF32        & 0x3F) | 0x80);
			} else {
				nLength += sizeof(uint8_t) * 6;
				if(nLength > nBufferLength) {
					break;	// バッファ足りない
				}
				*pUTF8++ = (uint8_t)( (*pUTF32 >> 30)         | 0xFC);
				*pUTF8++ = (uint8_t)(((*pUTF32 >> 24) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(((*pUTF32 >> 18) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(((*pUTF32 >> 12) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(((*pUTF32 >>  6) & 0x3F) | 0x80);
				*pUTF8++ = (uint8_t)(( *pUTF32        & 0x3F) | 0x80);
			}
			pUTF32++;
		}
		*pUTF8 = 0;
	} else {
		while(*pUTF32) {
			if(*pUTF32 <= 0x0000007F) {
				nLength += 1;
			} else if(*pUTF32 <= 0x000007FF) {
				nLength += 2;
			} else if(*pUTF32 <= 0x0000FFFF) {
				nLength += 3;
			} else if(*pUTF32 <= 0x001FFFFF) {
				nLength += 4;
			} else if(*pUTF32 <= 0x03FFFFFF) {
				nLength += 5;
			} else {
				nLength += 6;
			}
			pUTF32++;
		}
	}
	return nLength;
}

//! UTF-8からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF8			変換する文字列(UTF-8)
	@param[out]	pUTF16			変換された文字列(UTF-16) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF16に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF8toUTF16( const uint8_t* pUTF8, uint16_t* pUTF16, size_t nBufferLength )
{
	size_t nUTF16Length;
	size_t nUTF32Length;
	uint32_t* pTmp;

	if(pUTF8 == 0) {
		return 0;
	}
	nUTF32Length = Cat_UTF8toUTF32( pUTF8, 0, 0 );
	if(nUTF32Length == 0) {
		return 0;
	}
	pTmp = (uint32_t*)MALLOC( nUTF32Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF8toUTF32( pUTF8, pTmp, nUTF32Length );
	nUTF16Length = Cat_UTF32toUTF16( pTmp, pUTF16, nBufferLength );
	FREE( pTmp );
	return nUTF16Length;
}

//! UTF-16からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを \n
	考慮すること。

	@param[in]	pUTF16			変換する文字列(UTF-16)
	@param[out]	pUTF8			変換された文字列(UTF-8) (0を渡すことも可)
	@param[in]	nBufferLength	変換された文字列を格納するバッファサイズ(バイト単位) \n
								pUTF8に0を渡した場合は、無視される。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_UTF16toUTF8( const uint16_t* pUTF16, uint8_t* pUTF8, size_t nBufferLength )
{
	size_t nUTF8Length;
	size_t nUTF32Length;
	uint32_t* pTmp;

	if(pUTF16 == 0) {
		return 0;
	}
	nUTF32Length = Cat_UTF16toUTF32( pUTF16, 0, 0 );
	if(nUTF32Length == 0) {
		return 0;
	}
	pTmp = (uint32_t*)MALLOC( nUTF32Length );
	if(pTmp == 0) {
		return 0;
	}
	Cat_UTF16toUTF32( pUTF16, pTmp, nUTF32Length );
	nUTF8Length = Cat_UTF32toUTF8( pTmp, pUTF8, nBufferLength );
	FREE( pTmp );
	return nUTF8Length;
}
