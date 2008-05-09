//! @file Cat_GetButtonSwapInfo.h
// 決定ボタンのスワップ情報を取得する

#ifndef INCL_Cat_GetButtonSwapInfo_h
#define INCL_Cat_GetButtonSwapInfo_h

#ifdef __cplusplus
extern "C" {
#endif

//! 決定ボタンのスワップ情報
enum CAT_BUTTON_SWAP_INFO {
	CAT_BUTTON_SWAP_CIRCLE = 0,		/*!< ○ボタンが決定の環境 */
	CAT_BUTTON_SWAP_CROSS  = 1,		/*!< ×ボタンが決定の環境 */
};

//! 決定ボタンのスワップ情報を取得する
/*!
	フラッシュの設定から決定ボタンが○ボタンに割り当てられているか、×ボタンに割り当てられているかを取得する。 \n
	ファームウェアバージョンが1.0の場合は、言語設定から判断して値を返す。

	@return	○ボタンが決定の環境の場合は、\a CAT_BUTTON_SWAP_CIRCLE \n
			×ボタンが決定の環境の場合は、\a CAT_BUTTON_SWAP_CROSS \n
			が返る。
*/
extern int Cat_GetButtonSwap( void );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_GetButtonSwapInfo_h
