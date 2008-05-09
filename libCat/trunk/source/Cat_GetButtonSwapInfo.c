//! @file Cat_GetButtonSwapInfo.c
// 決定ボタンのスワップ情報を取得する

#include <pspkerneltypes.h>
#include <psputils.h>
#include <psputility.h>
#include "Cat_GetButtonSwapInfo.h"

//! 決定ボタンのスワップ情報を取得する
/*!
	フラッシュの設定から決定ボタンが○ボタンに割り当てられているか、×ボタンに割り当てられているかを取得する。 \n
	ファームウェアバージョンが1.0の場合は、言語設定から判断して値を返す。

	@return	○ボタンが決定の環境の場合は、\a CAT_BUTTON_SWAP_CIRCLE \n
			×ボタンが決定の環境の場合は、\a CAT_BUTTON_SWAP_CROSS \n
			が返る。
*/
int
Cat_GetButtonSwap( void )
{
	int nButtonSwap;
	int rc;

	rc = sceUtilityGetSystemParamInt( PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &nButtonSwap ); /* X/O button swap */
	if(rc == 0) {
		/* ボタンの入れ替え情報が取得できたら、それに従う */
		if(nButtonSwap == 0) {
			rc = CAT_BUTTON_SWAP_CIRCLE;
		} else {
			rc = CAT_BUTTON_SWAP_CROSS;
		}
	} else {
		/* 言語設定で判定する。多分1.0の場合 */
		int nLang;
		rc = sceUtilityGetSystemParamInt( PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &nLang );
		if(rc) {
			/* 言語情報も取得できなかったら英語にしとく */
			nLang = PSP_SYSTEMPARAM_LANGUAGE_ENGLISH;
		}
		switch(nLang) {
			case PSP_SYSTEMPARAM_LANGUAGE_JAPANESE:
			case PSP_SYSTEMPARAM_LANGUAGE_KOREAN:
			case PSP_SYSTEMPARAM_LANGUAGE_CHINESE_TRADITIONAL:
			case PSP_SYSTEMPARAM_LANGUAGE_CHINESE_SIMPLIFIED:
				rc = CAT_BUTTON_SWAP_CIRCLE;
				break;
			default:
				rc = CAT_BUTTON_SWAP_CROSS;
				break;
		}
	}
	return rc;
}
