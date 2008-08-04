//! @file	Cat_StreamPad.h
// 入力ストリーム関連

#ifndef INC_Cat_StreamPad_h
#define INC_Cat_StreamPad_h

#include "Cat_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

//! パッドストリームを開く
/*!
	@return	作成されたストリーム \n
			失敗した場合は、0が返る。
*/
extern Cat_Stream* Cat_StreamPadReadOpen( void );

#ifdef __cplusplus
}
#endif

#endif // INC_Cat_StreamPad_h
