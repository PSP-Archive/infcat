//! @file Cat_Big5Convert.h
// 繁体字中国語 Big5（Traditional Chinese）とUTFの変換を行う

#ifndef INCL_Cat_Big5Convert_h
#define INCL_Cat_Big5Convert_h

#include <stdlib.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

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
extern size_t Cat_Big5toUTF16( const char* pBig5, uint16_t* pUTF16, size_t nBufferLength );

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
extern size_t Cat_UTF16toBig5( const uint16_t* pUTF16, char* pBig5, size_t nBufferLength );

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
extern size_t Cat_Big5toUTF8( const char* pBig5, uint8_t* pUTF8, size_t nBufferLength );

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
extern size_t Cat_Big5toUTF32( const char* pBig5, uint32_t* pUTF32, size_t nBufferLength );

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
extern size_t Cat_UTF8toBig5( const uint8_t* pUTF8, char* pBig5, size_t nBufferLength );

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
extern size_t Cat_UTF32toBig5( const uint32_t* pUTF32, char* pBig5, size_t nBufferLength );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_Big5Convert_h
