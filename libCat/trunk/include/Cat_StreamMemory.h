//! @file	Cat_StreamMemory.h
// メモリストリーム関連

#ifndef INC_Cat_StreamMemory_h
#define INC_Cat_StreamMemory_h

#include <stdint.h>
#include "Cat_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

//! 読み込み専用のメモリストリームを開く
/*!
	@param[in]	pvData	データへのポインタ
	@param[in]	nSize	データサイズ
	@param[in]	fFree	クローズ時データを解放するかどうか 0以外で解放する
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
extern Cat_Stream* Cat_StreamMemoryReadOpen( void* pvData, int64_t nSize, uint32_t fFree );

#ifdef __cplusplus
}
#endif

#endif // INC_Cat_StreamMemory_h
