//! @file	Cat_Input.h
// 入力関連

#ifndef INCL_Cat_Input_h
#define INCL_Cat_Input_h

#include <stdint.h>
#include "Cat_Stream.h"

#ifdef __cplusplus
extern "C" {
#endif

//! 最大入力数
#define CAT_INPUT_MAX (32)

//! 最大レイテンシ数(2の乗数)
#define CAT_INPUT_MAX_LAYTENCE (1 << 6)

//! ボタン定義
enum {
	CAT_INPUT_SELECT    = 0x0001,	/*!< Selectボタン		*/
	CAT_INPUT_START     = 0x0008,	/*!< Startボタン		*/
	CAT_INPUT_UP        = 0x0010,	/*!< Upボタン			*/
	CAT_INPUT_RIGHT     = 0x0020,	/*!< Rightボタン		*/
	CAT_INPUT_DOWN      = 0x0040,	/*!< Downボタン			*/
	CAT_INPUT_LEFT      = 0x0080,	/*!< Leftボタン			*/
	CAT_INPUT_L1TRIGGER = 0x0100,	/*!< 左1トリガー		*/
	CAT_INPUT_R1TRIGGER = 0x0200,	/*!< 右1トリガー		*/
	CAT_INPUT_L2TRIGGER = 0x0400,	/*!< 左2トリガー		*/
	CAT_INPUT_R2TRIGGER = 0x0800,	/*!< 右2トリガー		*/
	CAT_INPUT_TRIANGLE  = 0x1000,	/*!< △ボタン			*/
	CAT_INPUT_CIRCLE    = 0x2000,	/*!< ○ボタン			*/
	CAT_INPUT_CROSS     = 0x4000,	/*!< ×ボタン			*/
	CAT_INPUT_SQUARE    = 0x8000,	/*!< □ボタン			*/
};

//! 初期化処理
extern void Cat_InputInit( void );

//! 終了処理
extern void Cat_InputTerm( void );

//! キー入力を行うストリームを設定する
/*!
	ストリームのデータフォーマットは、2バイトのリトルエンディアン \n
	Cat_InputUpdate()関数を呼び出したときに、ストリームより2バイト読み込みが行われ、 \n
	自分の入力データは、自分以外の他のストリームへの書き込みが行われる。 \n
	\n
	Cat_InputUpdate()を呼び出す前に設定すること。 \n
	デフォルトは、プレイヤ番号0にPadからの入力(Cat_StreamPad)が設定されている。

	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@param[in]	pStream			ストリーム
	@param[in]	fClose			終了処理時にストリームをクローズするかどうかの設定 \n
								0以外で終了処理時にストリームをクローズする。
	@see	Cat_InputUpdate()
*/
extern void Cat_InputSetStream( uint32_t nPlayerIndex, Cat_Stream* pStream, int32_t fClose );

//! 入力のレイテンシを設定する
/*!
	Cat_InputUpdate()を呼び出す前に設定すること。 \n
	デフォルトは、0に設定されている。
	@param[in]	nLaytence	レイテンシ(0 ～ CAT_INPUT_MAX_LAYTENCE-1)
*/
extern void Cat_InputSetLaytence( uint32_t nLaytence );

//! 自分のプレイヤ番号を設定する
/*!
	Cat_InputUpdate()を呼び出す前に設定すること。 \n
	デフォルトは、プレイヤ番号0が設定されている。
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
*/
extern void Cat_InputSetMyPlayerIndex( uint32_t nPlayerIndex );

//! キー情報を更新する
extern void Cat_InputUpdate( void );

//! 現在のキーを取得する
/*!
	無効なプレイヤ番号が設定された場合は、0を返す
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
extern uint32_t Cat_InputGetCurrent( uint32_t nPlayerIndex );

//! １つ前のキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
extern uint32_t Cat_InputGetPrevious( uint32_t nPlayerIndex );

//! 押下されたキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
extern uint32_t Cat_InputGetPressed( uint32_t nPlayerIndex );

//! 離されたキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
extern uint32_t Cat_InputGetRelease( uint32_t nPlayerIndex );

//! 変化のあったキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
extern uint32_t Cat_InputGetTrigger( uint32_t nPlayerIndex );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_Input_h
