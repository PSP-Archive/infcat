//! @file	Cat_EUCJPConvert.h
// EUC-JPとUTFの変換を行う

#ifndef INCL_Cat_EUCJPConvert_h
#define INCL_Cat_EUCJPConvert_h

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

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
extern size_t Cat_EUCJPtoUTF16( const char* pEUCJP, uint16_t* pUTF16, size_t nBufferLength );

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
extern size_t Cat_EUCJPtoUTF8( const char* pEUCJP, uint8_t* pUTF8, size_t nBufferLength );

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
extern size_t Cat_EUCJPtoUTF32( const char* pEUCJP, uint32_t* pUTF32, size_t nBufferLength );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_EUCJPConvert_h
