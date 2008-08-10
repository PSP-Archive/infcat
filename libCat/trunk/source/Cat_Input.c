//! @file	Cat_Input.h
// 入力関連

#include "Cat_Input.h"
#include "Cat_StreamPad.h"
#include <string.h>
#include <malloc.h>	// for memalign

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! キー状態
typedef struct {
	uint32_t	nCurrent;										/*!< 現在のキー							*/
	uint32_t	nPrevious;										/*!< １つ前のキー						*/
	uint32_t	nTrigger;										/*!< 変化のあったキー					*/
	uint32_t	nPressed;										/*!< 押下されたキー						*/
	uint32_t	nRelease;										/*!< 離されたキー						*/
} Cat_InputKeyState;

//! 入力ストリーム
typedef struct {
	Cat_Stream*			pStream;								/*!< ストリーム							*/
	int32_t				fClose;									/*!< 終了処理時にクローズするかどうか	*/
	Cat_InputKeyState	KeyState;								/*!< キー状態							*/
} Cat_InputStream;

//! 入力コンテキスト
typedef struct {
	uint32_t			nMyPlayerIndex;							/*!< 自分のプレイヤ番号					*/
	uint32_t			nLaytence;								/*!< レイテンシ	 						*/
	uint32_t			nCounter;								/*!< カウンタ							*/
	Cat_InputStream		InputStream[CAT_INPUT_MAX];				/*!< 入力								*/
	uint32_t			nReadPos;
	uint32_t			nWritePos;
	uint16_t			nMyKeyData[CAT_INPUT_MAX_LAYTENCE];
} Cat_InputContext;

//! 入力コンテキスト
static Cat_InputContext* gpInputContext = 0;

//! キー状態の更新
/*!
	@param[in]	pKeyState	更新するキー状態
	@param[in]	nKeyData	キーデータ
*/
static void
UpdateKeyState( Cat_InputKeyState* pKeyState, uint32_t nKeyData )
{
	pKeyState->nTrigger  = pKeyState->nCurrent ^ nKeyData;
	pKeyState->nPressed  = pKeyState->nTrigger & nKeyData;
	pKeyState->nRelease  = pKeyState->nTrigger & ~nKeyData;
	pKeyState->nPrevious = pKeyState->nCurrent;
	pKeyState->nCurrent  = nKeyData;
}

//! 初期化処理
void
Cat_InputInit( void )
{
	Cat_InputTerm();

	gpInputContext = (Cat_InputContext*)CAT_MALLOC( sizeof(Cat_InputContext) );
	if(gpInputContext) {
		memset( gpInputContext, 0, sizeof(Cat_InputContext) );

		// プレイヤ0番にPadを割り当てる
		Cat_InputSetMyPlayerIndex( 0 );
		Cat_InputSetStream( 0, Cat_StreamPadReadOpen(), 1 );
	}
}

//! 終了処理
void
Cat_InputTerm( void )
{
	if(gpInputContext) {
		int32_t i;
		for(i = 0; i < CAT_INPUT_MAX; i++) {
			if(gpInputContext->InputStream[i].pStream && gpInputContext->InputStream[i].fClose) {
				Cat_StreamClose( gpInputContext->InputStream[i].pStream );
			}
		}
		memset( gpInputContext, 0, sizeof(Cat_InputContext) );
		CAT_FREE( gpInputContext );
		gpInputContext = 0;
	}
}

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
void
Cat_InputSetStream( uint32_t nPlayerIndex, Cat_Stream* pStream, int32_t fClose )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return;
	}

	if(gpInputContext->InputStream[nPlayerIndex].pStream && gpInputContext->InputStream[nPlayerIndex].fClose) {
		Cat_StreamClose( gpInputContext->InputStream[nPlayerIndex].pStream );
	}
	gpInputContext->InputStream[nPlayerIndex].pStream = pStream;
	gpInputContext->InputStream[nPlayerIndex].fClose  = fClose;
	memset( &gpInputContext->InputStream[nPlayerIndex].KeyState, 0, sizeof(Cat_InputKeyState) );
}

//! 入力のレイテンシを設定する
/*!
	Cat_InputUpdate()を呼び出す前に設定すること。 \n
	デフォルトは、0に設定されている。
	@param[in]	nLaytence	レイテンシ(0 ～ CAT_INPUT_MAX_LAYTENCE-1)
*/
void
Cat_InputSetLaytence( uint32_t nLaytence )
{
	gpInputContext->nLaytence = nLaytence;
	//gpInputContext->nReadPos  = 0;
	gpInputContext->nWritePos = (gpInputContext->nReadPos + gpInputContext->nLaytence) & (CAT_INPUT_MAX_LAYTENCE - 1);
}

//! 自分のプレイヤ番号を設定する
/*!
	Cat_InputUpdate()を呼び出す前に設定すること。 \n
	デフォルトは、プレイヤ番号0が設定されている。
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
*/
void
Cat_InputSetMyPlayerIndex( uint32_t nPlayerIndex )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return;
	}
	gpInputContext->nMyPlayerIndex = nPlayerIndex;
}

//! キー情報を更新する
void
Cat_InputUpdate( void )
{
	uint32_t i;
	uint16_t nKeyData;
	uint16_t nMyKeyData;

	// 自分のを読み込む
	nMyKeyData = 0;
	if(Cat_StreamRead( gpInputContext->InputStream[gpInputContext->nMyPlayerIndex].pStream, &nMyKeyData, 2 ) < 0) {
		nMyKeyData = 0;
	}
	gpInputContext->nMyKeyData[gpInputContext->nWritePos] = nMyKeyData;

	if(gpInputContext->nLaytence <= gpInputContext->nCounter) {
		// データを読み込む
		for(i = 0; i < CAT_INPUT_MAX; i++) {
			if(i != gpInputContext->nMyPlayerIndex) {
				nKeyData = 0;
				if(Cat_StreamRead( gpInputContext->InputStream[i].pStream, &nKeyData, 2 ) < 0) {
					nKeyData = 0;
				}
			} else {
				nKeyData = gpInputContext->nMyKeyData[gpInputContext->nReadPos];
			}
			UpdateKeyState( &gpInputContext->InputStream[i].KeyState, nKeyData );
		}
	}

	// ストリームへ書き込む
	for(i = 0; i < CAT_INPUT_MAX; i++) {
		if(gpInputContext->InputStream[i].pStream) {
			Cat_StreamWrite( gpInputContext->InputStream[i].pStream, &nMyKeyData, 2 );
		}
	}

	// 更新
	gpInputContext->nWritePos = (gpInputContext->nWritePos + 1) & (CAT_INPUT_MAX_LAYTENCE - 1);
	gpInputContext->nReadPos  = (gpInputContext->nReadPos  + 1) & (CAT_INPUT_MAX_LAYTENCE - 1);
	gpInputContext->nCounter++;
}

//! 現在のキーを取得する
/*!
	無効なプレイヤ番号が設定された場合は、0を返す
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
uint32_t Cat_InputGetCurrent( uint32_t nPlayerIndex )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return 0;
	}
	return gpInputContext->InputStream[nPlayerIndex].KeyState.nCurrent;
}

//! １つ前のキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
uint32_t
Cat_InputGetPrevious( uint32_t nPlayerIndex )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return 0;
	}
	return gpInputContext->InputStream[nPlayerIndex].KeyState.nPrevious;
}

//! 押下されたキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
uint32_t
Cat_InputGetPressed( uint32_t nPlayerIndex )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return 0;
	}
	return gpInputContext->InputStream[nPlayerIndex].KeyState.nPressed;
}

//! 離されたキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
uint32_t
Cat_InputGetRelease( uint32_t nPlayerIndex )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return 0;
	}
	return gpInputContext->InputStream[nPlayerIndex].KeyState.nRelease;
}

//! 変化のあったキーを取得する
/*!
	@param[in]	nPlayerIndex	プレイヤ番号(0～31)
	@return キー状態
*/
uint32_t
Cat_InputGetTrigger( uint32_t nPlayerIndex )
{
	if(nPlayerIndex >= CAT_INPUT_MAX) {
		return 0;
	}
	return gpInputContext->InputStream[nPlayerIndex].KeyState.nTrigger;
}
