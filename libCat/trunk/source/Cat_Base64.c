//! @file	Cat_Base64.c
// BASE64変換

//! @todo	コーディングのみ。テストすること。

#include <inttypes.h>
#include <netinet/in.h>

//! BASE64のエンコードテーブル
static const uint8_t tblBase64Encode[0x40] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

//! BASE64のデコードテーブル
static const uint8_t tblBase64Decode[0x100] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
	0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

//! 3バイト分エンコードする
static void
Encode3bytes( uint32_t nCode, uint8_t* pDst )
{
	// 0000 0000 0000 0000 0000 0000 .... .... ; 上位24bit
	// aaaa aa                                 ; 1文字目
	//        bb bbbb                          ; 2文字目
	//                cccc cc                  ; 3文字目
	//                       dd dddd           ; 4文字目
#ifdef __psp__
	((struct { uint32_t n32; } __attribute__((__packed__))*)pDst)->n32 =
		   (uint32_t)tblBase64Encode[(nCode >> 26)       ]			// 1文字目
		| ((uint32_t)tblBase64Encode[(nCode >> 20) & 0x3F] <<  8)		// 2文字目
		| ((uint32_t)tblBase64Encode[(nCode >> 14) & 0x3F] << 16)		// 3文字目
		| ((uint32_t)tblBase64Encode[(nCode >>  8) & 0x3F] << 24);	// 4文字目
#else
	pDst[0] = (uint32_t)tblBase64Encode[(nCode >> 26)       ];	// 1文字目
	pDst[1] = (uint32_t)tblBase64Encode[(nCode >> 20) & 0x3F];	// 2文字目
	pDst[2] = (uint32_t)tblBase64Encode[(nCode >> 14) & 0x3F];	// 3文字目
	pDst[3] = (uint32_t)tblBase64Encode[(nCode >>  8) & 0x3F];	// 4文字目
#endif
}

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
size_t
Cat_Base64Encode( const void* pvData, size_t nDataSize, char* pszBase64 )
{
	if(pvData == 0) {
		return 0;
	}
	if(pszBase64) {
		uint8_t* pbBuffer = (uint8_t*)pszBase64;
		uint8_t* pbData   = (uint8_t*)pvData;
		uint32_t nCode;
		while(nDataSize >= 3) {
			nCode = htonl( ((const struct { uint32_t n32; } __attribute__((__packed__))*)pbData)->n32 );
			Encode3bytes( nCode, pbBuffer );
			pbData    += 3;
			nDataSize -= 3;
			pbBuffer  += 4;
		}
		// 残りがあったらそれを処理する
		if(nDataSize == 1) {
			nCode  = ((uint32_t)*pbData) << 24;
			Encode3bytes( nCode, pbBuffer );
			pbBuffer[2] = '=';
			pbBuffer[3] = '=';
			pbBuffer += 4;
		} else if(nDataSize == 2) {
			nCode  = ((uint32_t)*pbData++) << 24;
			nCode |= ((uint32_t)*pbData)   << 16;
			Encode3bytes( nCode, pbBuffer );
			pbBuffer[3] = '=';
			pbBuffer += 4;
		}
		*pbBuffer = 0;
		return pbBuffer - (uint8_t*)pszBase64 + 1;
	} else {
		// 変換に必要なサイズを計算して返す
		return ((nDataSize + 2) / 3) * 4 + 1;
	}
}

//! デコード後に必要なバッファサイズを計算する
/*!
	@param[in]	pszBase64	BASE64の文字列
	@return	変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
static size_t
CalcDecodeSize( const char* pszBase64 )
{
	// 文字列長を調べる
	// 文字列に不正な文字が入ってないかも調べる
	size_t nLength = 0;
	while(pszBase64[nLength]) {
		if(tblBase64Decode[nLength] >= 0x40) {
			return 0;	// 不正な文字が含まれていた
		}
		nLength++;
	}
	if(nLength == 0 || (nLength & 0x3)) {
		return 0;	// 4の倍数でないなら、不正なデータとみなす
	}

	nLength = (nLength / 4) * 3;

	// 端数の処理
	if(pszBase64[nLength - 1] == '=') {
		if(pszBase64[nLength - 2] == '=') {
			nLength -= 2;
		} else {
			nLength -= 1;
		}
	}
	return nLength;
}

//! BASE64からデータへ変換する
/*!
	\a pvData に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@param[in]	pszBase64	変換するBASE64文字列(NULLターミネイトされていること)
	@param[out]	pvData		デコードされたデータを格納するポインタ
	@param[in]	nDataSize	デコードされたデータを格納するバッファサイズ(バイト単位)
	@return	変換に必要なバッファのサイズ(バイト単位) \n
			エラーの場合は、0が返る。
*/
size_t
Cat_Base64Decode( const char* pszBase64, void* pvData, size_t nDataSize )
{
	size_t nSrcLength;
	if(pszBase64 == 0) {
		return 0;
	}

	// 変換に必要なサイズを取得
	nSrcLength = CalcDecodeSize( pszBase64 );
	if(nSrcLength == 0) {
		return 0;
	}

	if(pvData) {
		uint8_t* pbBuffer;
		if(nSrcLength > nDataSize) {
			return 0;	// バッファが足りない
		}
		nDataSize = nSrcLength;
		pbBuffer = (uint8_t*)pvData;
		while(nDataSize > 3) {
			uint32_t nCode;
			// .... .... 0000 0000 0000 0000 0000 0000 ; 下位24bit
			//           aaaa aa                       ; 1文字目
			//                  bb bbbb                ; 2文字目
			//                          cccc cc        ; 3文字目
			//                                 dd dddd ; 4文字目
			nCode  = (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] << 18;
			nCode |= (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] << 12;
			nCode |= (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] <<  6;
			nCode |= (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++];
			*pbBuffer++ = (uint8_t)( nCode >> 16);
			*pbBuffer++ = (uint8_t)((nCode >>  8) & 0xFF);
			*pbBuffer++ = (uint8_t)( nCode        & 0xFF);
			nDataSize -= 3;
		}
		// 端数の処理
		if(nDataSize == 1) {
			uint32_t nCode;
			// .... .... 0000 0000 0000 0000 0000 0000 ; 下位24bit
			//           aaaa aa                       ; 1文字目
			//                  bb bbbb                ; 2文字目
			nCode  = (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] << 18;
			nCode |= (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] << 12;
			*pbBuffer = (uint8_t)( nCode >> 16);
		} else if(nDataSize == 2) {
			uint32_t nCode;
			// .... .... 0000 0000 0000 0000 0000 0000 ; 下位24bit
			//           aaaa aa                       ; 1文字目
			//                  bb bbbb                ; 2文字目
			//                          cccc cc        ; 3文字目
			nCode  = (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] << 18;
			nCode |= (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] << 12;
			nCode |= (uint32_t)tblBase64Decode[*(uint8_t*)pszBase64++] <<  6;
			*pbBuffer++ = (uint8_t)( nCode >> 16);
			*pbBuffer   = (uint8_t)((nCode >>  8) & 0xFF);
		}
	}
	return nSrcLength;
}
