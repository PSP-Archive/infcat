//! @file	Cat_Base64.h
// BASE64変換

#ifndef INCL_Cat_Base64_h
#define INCL_Cat_Base64_h

#include <inttypes.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//! データをBASE64へ変換する
/*!
	\a pszBase64 に0を渡すと、変換に必要なバッファサイズを計算することができる。 \n
	変換された文字列はNULLターミネイトされるので、バッファサイズにはそれを考慮 \n
	すること。

	@param[in]	pvData		変換するデータ
	@param[in]	nDataSize	変換するデータのサイズ(バイト単位)
	@param[out]	pszBase64	BASE64に変換された文字列を格納するバッファサイズ(バイト単位) \n
							NULLターミネイトを含む。
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
extern size_t Cat_Base64Encode( const void* pvData, size_t nDataSize, char* pszBase64 );

//! BASE64からデータへ変換する
/*!
	\a pvData に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pszBase64	変換するBASE64文字列(NULLターミネイトされていること)
	@param[out]	pvData		デコードされたデータを格納するポインタ
	@param[in]	nDataSize	デコードされたデータを格納するバッファサイズ(バイト単位)
	@return	変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
extern size_t Cat_Base64Decode( const char* pszBase64, void* pvData, size_t nDataSize );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_Base64_h
