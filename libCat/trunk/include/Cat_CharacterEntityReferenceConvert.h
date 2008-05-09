//! @file	Cat_CharacterEntityReferenceConvert.h
// 文字実体参照(Character entity reference)の変換をする

#ifndef INCL_Cat_CharacterEntityReferenceConvert_h
#define INCL_Cat_CharacterEntityReferenceConvert_h

#ifdef __cplusplus
extern "C" {
#endif

//! 文字実体参照(Character entity reference)をデコードする
/*!
	\a pszText に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@code
		// "&lt;abcde&gt;" → "<abcde>"
		int nSize = Cat_CharacterEntityReferenceDecode( "&lt;abcde&gt;", 0 );	// 必要なバッファサイズを取得
		char* pszText = malloc( nSize );
		Cat_CharacterEntityReferenceDecode( "&lt;abcde&gt;", pszText );			// 実際に変換
	@endcode

	@param[in]	pszHTML	デコードする文字列(UTF-8)
	@param[out]	pszText	デコードされた文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_CharacterEntityReferenceDecode( const char* pszHTML, char* pszText );

//! 文字実体参照(Character entity reference)へエンコードする
/*!
	\a pszHTML に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@code
		// "<abcde>" → "&lt;abcde&gt;"
		int nSize = Cat_CharacterEntityReferenceEncode( "<abcde>", 0 );	// 必要なバッファサイズを取得
		char* pszHTML = malloc( nSize );
		Cat_CharacterEntityReferenceEncode( "<abcde>", pszHTML );		// 実際に変換
	@endcode

	@param[in]	pszText	エンコードする文字列(UTF-8)
	@param[out]	pszHTML	エンコードされた文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_CharacterEntityReferenceEncode( const char* pszText, char* pszHTML );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_CharacterEntityReferenceConvert_h
