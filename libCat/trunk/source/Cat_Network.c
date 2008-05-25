//! @file Cat_Network.c
// ネットワークの初期化と接続処理など

/*
	void draw_callback(void* pvUserData ) {
		// 描画処理
		// ダイアログ表示のさいの背景を描画する
		sceGuStart() ～ sceGuFinish()
	}

	void screen_update_callback(void* pvUserData ) {
		// 更新処理
		// フレームバッファのスワップなど
		sceGuSync( 0, 0 ); sceDisplayWaitVblankStartCB(); sceGuSwapBuffers(); など
	}


	// ネットワークの初期化
	Cat_NetworkInit();
	...
	...

	// 接続する
	pvUserData = 0; // コールバック関数での第一引数となる
	rc = Cat_NetworkConnect( draw_callback, screen_update_callback, pvUserData );
	if(rc < 0) {
		error
	} else if(rc > 0) {
		cancel
	} else {
		connect
	}

	...
	...
	// ネットワークの終了処理
	Cat_NetworkTerm();

注意
 ・ネットワークモジュールをあらかじめ読み込んでおく必要がある
 ・1.0/1.50はkernelModeなスレッドで読み込んでおくこと

*/

#include <pspkerneltypes.h>
#include <pspthreadman.h>
#include <pspkerror.h>
#include <psputils.h>
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <pspnet_resolver.h>
#include <pspwlan.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "Cat_Network.h"
#include "Cat_Resolver.h"
#include "Cat_GetButtonSwapInfo.h"

//! 接続終了後に自動的にリゾルバエンジンを初期化するかどうか
#define AUTO_INIT_RESOLVER_ENGINE 1

//! メモリ確保マクロ
#define MALLOC(x) malloc(x)
//! メモリ解放マクロ
#define FREE(x) free(x)
//! 文字列複製マクロ
#define STRDUP(x) strdup(x)

#ifdef DEBUG_TRACE
#define TRACE(x) pspDebugScreenPrintf x
#else
#define TRACE(x)
#endif

//! 初期化済フラグ
static int gfInitialized = 0;

//! ネットワークの初期化
/*!
	ネットワークモジュールをあらかじめ読み込んでおく必要がある。
	@return	正常終了時、0。\n
			初期化に失敗した場合は、負数のエラーコードが返る。
*/
int
Cat_NetworkInit( void )
{
	int rc;

	if(gfInitialized == 0) {

		rc = sceNetInit( 256*1024, 0x12, 0, 0x12, 0 );
		if(rc < 0) {
			rc = sceNetInit( 256*1024, 0x12, 0x1000, 0x12, 0x1000 );
		}
		if(rc < 0) {
			return rc;
		}

		rc = sceNetInetInit();
		if(rc < 0) {
			sceNetTerm();
			return rc;
		}

		rc = sceNetApctlInit( 0x8000, 0x13 );
		if(rc < 0) {
			sceNetInetTerm();
			sceNetTerm();
			return rc;
		}
		gfInitialized = 1;
	}

	return 0;
}

//! ネットワークの切断と終了処理
/*!
	@see	Cat_NetworkInit()
*/
void
Cat_NetworkTerm( void )
{
	if(gfInitialized) {
		sceNetApctlDisconnect();
		while(Cat_NetworkIsConnect()) {
			sceKernelDelayThreadCB( 100 * 1000 );
		}
		sceNetApctlTerm();
		sceNetInetTerm();
		sceNetTerm();

		gfInitialized = 0;
	}
}

// ダイアログ共通部分の初期化
static void
ConfigureDialog( pspUtilityMsgDialogParams* dialog, size_t dialog_size )
{
	memset( dialog, 0, dialog_size );

	dialog->base.size = dialog_size;
	sceUtilityGetSystemParamInt( PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &dialog->base.language );  // Prompt language
	dialog->base.buttonSwap = Cat_GetButtonSwap(); /* X/O button swap */

	dialog->base.graphicsThread = 0x11;
	dialog->base.accessThread   = 0x13;
	dialog->base.fontThread     = 0x12;
	dialog->base.soundThread    = 0x10;
}

// エラーダイアログの表示開始
static int
ShowMessageDialogError( int nErrorCode )
{
	static pspUtilityMsgDialogParams dialog;

	ConfigureDialog(&dialog, sizeof(dialog));
	dialog.mode       = 0;
	dialog.errorValue = nErrorCode;

	return sceUtilityMsgDialogInitStart( &dialog );
}

//! ネットワーク接続処理
/*!
	標準のダイアログを使いネットワークへの接続を行う。\n
	この関数を呼び出す前に、ネットワークモジュールの読み込みをし、
	Cat_NetworkInit()で初期化を行っている必要がある。

	@code
	void draw_callback(void* pvUserData ) {
		// 描画処理
		// ダイアログ表示のさいの背景を描画する
		sceGuStart() ～ sceGuFinish()
	}
	void screen_update_callback(void* pvUserData ) {
		// 更新処理
		// フレームバッファのスワップなど
		sceGuSync( 0, 0 ); sceDisplayWaitVblankStartCB(); sceGuSwapBuffers(); など
	}

		// ネットワークの初期化
		Cat_NetworkInit();
		...
		...
		// 接続する
		pvUserData = 0; // コールバック関数での第一引数となる
		rc = Cat_NetworkConnect( draw_callback, screen_update_callback, pvUserData );
		if(rc < 0) {
			// error
		} else if(rc > 0) {
			// cancel
		} else {
			// connect
		}
		...
		...
		// ネットワークの終了処理
		Cat_NetworkTerm();
	@endcode

	@param[in]	draw_callback			描画用コールバック関数
	@param[in]	screen_update_callback	更新用コールバック関数
	@param[in]	pvUserData				コールバック関数に渡されるユーザデータ
	@return	接続に成功した場合は、0が返る。\n
			キャンセルされた場合は、正数が返る。\n
			エラーが発生した場合は、負数のエラーコードが返る。
	@see	Cat_NetworkInit()
	@see	Cat_NetworkTerm()
*/
int
Cat_NetworkConnect( void (*draw_callback)(void*), void (*screen_update_callback)(void*), void* pvUserData )
{
	static char pBuffer[sizeof(pspUtilityNetconfData) + 64];
	pspUtilityNetconfData* pConf;
	int nState;
	int rc;
	int fFinish;

	rc = Cat_NetworkInit();
	if(rc < 0) {
		return rc;
	}

	pConf = (pspUtilityNetconfData*)pBuffer;
	memset( pConf, 0, sizeof(pspUtilityNetconfData) + 64 );
	pConf->base.size = sizeof(pspUtilityNetconfData) + 64;

	sceUtilityGetSystemParamInt( PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pConf->base.language ); // Prompt language
	pConf->base.buttonSwap     = Cat_GetButtonSwap(); /* X/O button swap */
	pConf->action              = PSP_NETCONF_ACTION_CONNECTAP;
	pConf->base.graphicsThread = 0x11;
	pConf->base.accessThread   = 0x13;
	pConf->base.fontThread     = 0x12;
	pConf->base.soundThread    = 0x10;

	rc = sceUtilityNetconfInitStart( pConf );
	while((unsigned int)rc == 0x80110004U && pConf->base.size >= 4) {
		pConf->base.size = pConf->base.size - 4;
		rc = sceUtilityNetconfInitStart( pConf );
	}
	if(rc < 0) {
		return rc;
	}

	nState = 1;
	fFinish = 0;
	while(!fFinish) {
		switch(nState) {
			case 0:
				rc = sceUtilityNetconfInitStart( pConf );
				if(rc < 0) {
					nState = 10;
				}
				nState = 1;
				break;
			case 1:
				if(sceUtilityNetconfGetStatus() == PSP_UTILITY_DIALOG_QUIT) {
					sceUtilityNetconfShutdownStart();
					nState = 2;
				}
				break;
			case 2:
				if(sceUtilityNetconfGetStatus() == PSP_UTILITY_DIALOG_NONE) {
					/* 接続完了 */
					rc = pConf->base.result;
					if(rc < 0) {
						/* エラーが発生した エラーメッセージでも出しとこう */
						nState = 10;
					} else {
						if(rc > 0) {
							/* キャンセルされたので */
							Cat_NetworkTerm();
						}
#if AUTO_INIT_RESOLVER_ENGINE
						else {
							Cat_ResolverInitEngine();
						}
#endif // AUTO_INIT_RESOLVER_ENGINE
						fFinish = 1;
					}
				}
				break;

			case 10:
				rc = ShowMessageDialogError( rc );
				if(rc < 0) {
					/* エラーが発生した */
					fFinish = 1;
					Cat_NetworkTerm();
				} else {
					nState = 11;
				}
				break;
			case 11:
				if(sceUtilityMsgDialogGetStatus() == PSP_UTILITY_DIALOG_QUIT) {
					sceUtilityMsgDialogShutdownStart();
					nState = 12;
				}
				break;
			case 12:
				if(sceUtilityMsgDialogGetStatus() == PSP_UTILITY_DIALOG_NONE) {
					nState = 0;
				}
				break;
		}

		if(draw_callback) {
			draw_callback( pvUserData );
		}

		if(sceUtilityNetconfGetStatus() == PSP_UTILITY_DIALOG_VISIBLE) {
			sceUtilityNetconfUpdate( 2 );
		}
		if(sceUtilityMsgDialogGetStatus() == PSP_UTILITY_DIALOG_VISIBLE) {
			sceUtilityMsgDialogUpdate( 2 );
		}

		if(screen_update_callback) {
			screen_update_callback( pvUserData );
		}
	}
	return rc;
}

//! ネットワーク状態表示
/*!
	@param[in]	draw_callback			描画用コールバック関数
	@param[in]	screen_update_callback	更新用コールバック関数
	@param[in]	pvUserData				コールバック関数に渡されるユーザデータ
	@see	Cat_NetworkConnect()
*/
int
Cat_NetworkDisplayStatus( void (*draw_callback)(void*), void (*screen_update_callback)(void*), void* pvUserData )
{
	static char pBuffer[sizeof(pspUtilityNetconfData) + 64];
	pspUtilityNetconfData* pConf;
	int nState;
	int rc;
	int fFinish;

	pConf = (pspUtilityNetconfData*)pBuffer;
	memset( pConf, 0, sizeof(pspUtilityNetconfData) + 64 );
	pConf->base.size = sizeof(pspUtilityNetconfData) + 64;

	sceUtilityGetSystemParamInt( PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pConf->base.language ); // Prompt language
	pConf->base.buttonSwap     = Cat_GetButtonSwap(); /* X/O button swap */
	pConf->action              = PSP_NETCONF_ACTION_DISPLAYSTATUS;
	pConf->base.graphicsThread = 0x11;
	pConf->base.accessThread   = 0x13;
	pConf->base.fontThread     = 0x12;
	pConf->base.soundThread    = 0x10;

	rc = sceUtilityNetconfInitStart( pConf );
	while((unsigned int)rc == 0x80110004U && pConf->base.size >= 4) {
		pConf->base.size = pConf->base.size - 4;
		rc = sceUtilityNetconfInitStart( pConf );
	}
	nState = 1;
	if(rc < 0) {
		nState = 10;
	}

	fFinish = 0;
	while(!fFinish) {
		switch(nState) {
			case 1:
				if(sceUtilityNetconfGetStatus() == PSP_UTILITY_DIALOG_QUIT) {
					sceUtilityNetconfShutdownStart();
					nState = 2;
				}
				break;
			case 2:
				if(sceUtilityNetconfGetStatus() == PSP_UTILITY_DIALOG_NONE) {
					rc = 0;
					fFinish = 1;
				}
				break;

			case 10:
				if(ShowMessageDialogError( rc ) < 0) {
					/* エラーが発生した */
					fFinish = 1;
				} else {
					nState = 11;
				}
				break;
			case 11:
				if(sceUtilityMsgDialogGetStatus() == PSP_UTILITY_DIALOG_QUIT) {
					sceUtilityMsgDialogShutdownStart();
					nState = 12;
				}
				break;
			case 12:
				if(sceUtilityMsgDialogGetStatus() == PSP_UTILITY_DIALOG_NONE) {
					fFinish = 1;
				}
				break;
		}

		if(draw_callback) {
			draw_callback( pvUserData );
		}

		if(sceUtilityNetconfGetStatus() == PSP_UTILITY_DIALOG_VISIBLE) {
			sceUtilityNetconfUpdate( 2 );
		}
		if(sceUtilityMsgDialogGetStatus() == PSP_UTILITY_DIALOG_VISIBLE) {
			sceUtilityMsgDialogUpdate( 2 );
		}

		if(screen_update_callback) {
			screen_update_callback( pvUserData );
		}
	}
	return rc;
}

//! ネットワーク接続状態を取得
/*!
	@return	接続している状態なら1。そうでないなら0を返す。
*/
int
Cat_NetworkIsConnect( void )
{
	int nState;
	if(sceNetApctlGetState( &nState ) < 0) {
		return 0;
	}
	return (nState == 4) ? 1 : 0;
}
