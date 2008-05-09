//! @file	Cat_URLConvert.c
// URLコンバートする

#include <stdio.h>

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
int
Cat_URLEncode( const char* pszURL, char* pszText )
{
	int nLength;

	if(pszURL == 0) {
		return 0;
	}

	nLength = 0;
	while(*pszURL) {
		if(('a' <= *pszURL && *pszURL <= 'z')
			|| ('A' <= *pszURL && *pszURL <= 'Z')
			|| ('0' <= *pszURL && *pszURL <= '9')
			|| (*pszURL == '-')
			|| (*pszURL == '_')
			|| (*pszURL == '.')
			|| (*pszURL == '!')
			|| (*pszURL == '~')
			|| (*pszURL == '*')
			|| (*pszURL == '(')
			|| (*pszURL == ')')) {
			/* そのまま */
			if(pszText) {
				*pszText++ = *pszURL;
			}
			nLength++;
			pszURL++;
		} else if(*pszURL == ' ') {
			if(pszText) {
				*pszText++ = '+';
			}
			nLength++;
			pszURL++;
		} else {
			/* 変換 */
			if(pszText) {
				*pszText++ = '%';
				sprintf( pszText, "%02X", (unsigned int)(*(unsigned char*)pszURL) );
				pszText += 2;
			}
			pszURL++;
			nLength += 3;
		}
	}
	if(pszText) {
		*pszText = 0;
	}

	return nLength + 1;
}
