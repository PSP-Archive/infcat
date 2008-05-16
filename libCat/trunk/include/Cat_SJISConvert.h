//! @file	Cat_SJISConvert.h
// Shift-JISとUTFの変換を行う

#ifndef INCL_Cat_SJISConvert_h
#define INCL_Cat_SJISConvert_h

#include <stdlib.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

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
extern size_t Cat_SJIStoUTF16( const char* pSJIS, uint16_t* pUTF16, size_t nBufferLength );

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
extern size_t Cat_UTF16toSJIS( const uint16_t* pUTF16, char* pSJIS, size_t nBufferLength );

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
extern size_t Cat_SJIStoUTF8( const char* pSJIS, uint8_t* pUTF8, size_t nBufferLength );

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
extern size_t Cat_SJIStoUTF32( const char* pSJIS, uint32_t* pUTF32, size_t nBufferLength );

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
extern size_t Cat_UTF8toSJIS( const uint8_t* pUTF8, char* pSJIS, size_t nBufferLength );

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
extern size_t Cat_UTF32toSJIS( const uint32_t* pUTF32, char* pSJIS, size_t nBufferLength );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_SJISConvert_h
