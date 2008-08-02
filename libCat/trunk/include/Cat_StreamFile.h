//! @file	Cat_StreamFile.h
// ファイルストリーム関連

#ifndef INC_Cat_StreamFile_h
#define INC_Cat_StreamFile_h

#include <stdint.h>
#include "Cat_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

//! 読み込み専用でファイルを開く
/*!
	@param[in]	ファイル名
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
extern Cat_Stream* Cat_StreamFileReadOpen( const char* pszFilename );

//! 書き込み専用でファイルを開く
/*!
	@param[in]	ファイル名
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
extern Cat_Stream* Cat_StreamFileWriteOpen( const char* pszFilename );

#ifdef __cplusplus
}
#endif

#endif // INC_Cat_StreamFile_h
