//! @file	Cat_URLConvert.h
// URLコンバートする

#ifndef INCL_Cat_URLConvert_h
#define INCL_Cat_URLConvert_h

#ifdef __cplusplus
extern "C" {
#endif

//! URLエンコードする
/*!
	\a pszText に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@code
		int nSize = Cat_URLEncode( "<abcde>", 0 );	// 必要なバッファサイズを取得
		char* pszText = malloc( nSize );
		Cat_URLEncode( "<abcde>", pszText );		// 実際に変換
	@endcode

	@param[in]	pszURL	エンコードする文字列(UTF-8)
	@param[out]	pszText	エンコードされた文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_URLEncode( const char* pszURL, char* pszText );

#ifdef __cplusplus
}
#endif

#endif // Cat_URLConvert_h
