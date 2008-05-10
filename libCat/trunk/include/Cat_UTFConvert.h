//! @file	Cat_UTFConvert.h
// UTF-8,UTF-16,UTF-32の変換をする

#ifndef INCL_Cat_UTFConvert_h
#define INCL_Cat_UTFConvert_h

#ifdef __cplusplus
extern "C" {
#endif

//! UTF-16からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF16	変換する文字列(UTF-16)
	@param[out]	pUTF32	変換された文字列(UTF-32) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_UTF16toUTF32( const unsigned short* pUTF16, unsigned int* pUTF32 );

//! UTF-32からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF32	変換する文字列(UTF-32)
	@param[out]	pUTF16	変換された文字列(UTF-16) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_UTF32toUTF16( const unsigned int* pUTF32, unsigned short* pUTF16 );

//! UTF-8からUTF-32へ変換する
/*!
	\a pUTF32 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF8	変換する文字列(UTF-8)
	@param[out]	pUTF32	変換された文字列(UTF-32) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_UTF8toUTF32( const unsigned char* pUTF8, unsigned int* pUTF32 );

//! UTF-32からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF32	変換する文字列(UTF-32)
	@param[out]	pUTF8	変換された文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_UTF32toUTF8( const unsigned int* pUTF32, unsigned char* pUTF8 );

//! UTF-8からUTF-16へ変換する
/*!
	\a pUTF16 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF8	変換する文字列(UTF-8)
	@param[out]	pUTF16	変換された文字列(UTF-16) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_UTF8toUTF16( const unsigned char* pUTF8, unsigned short* pUTF16 );

//! UTF-16からUTF-8へ変換する
/*!
	\a pUTF8 に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pUTF16	変換する文字列(UTF-16)
	@param[out]	pUTF8	変換された文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
extern int Cat_UTF16toUTF8( const unsigned short* pUTF16, unsigned char* pUTF8 );

#ifdef __cplusplus
}
#endif

#endif // Cat_UTFConvert_h
