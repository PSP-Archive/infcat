//! @file Cat_PspCallback.c
// Homeキーによる終了コールバックの設定を行う

#include <pspthreadman.h>
#include <psploadexec.h>
#include "Cat_PspCallback.h"

//! homeキーコールバック名
#define HOME_CALLBACK_NAME		"h"

//! homeキーコールバック登録スレッド名
#define HOME_CALLBACK_THREAD_NAME		"u"

//! homeキーコールバック登録スレッドのスレッドプライオリティ
#define HOME_CALLBACK_THREAD_PRIORITY	(0x11)

//! homeキーコールバック登録スレッドのスタックサイズ
#define HOME_CALLBACK_THREAD_STACK_SIZE	(0xFA0)


//! homeキーが押下されたときに呼ばれるコールバック関数
/*!
	ゲームを終了し、XMBに戻る。\n
	このコールバック関数は、HomeKeyCallbackThreadで登録される。

	@param[in]	arg1	未使用
	@param[in]	arg2	未使用
	@param[in]	common	未使用
	@return	未使用
	@see	Cat_SetupCallbacks()
	@see	HomeKeyCallbackThread()
*/
static int
ExitCallback( int arg1, int arg2, void* common )
{
	sceKernelExitGame();
	return 0;
}

//! Homeキーコールバック登録スレッド
/*!
	@param[in]	args	未使用
	@param[in]	argp	未使用
	@return	未使用
	@see	Cat_SetupCallbacks()
	@see	ExitCallback()
*/
static int
HomeKeyCallbackThread( SceSize args, void *argp )
{
	sceKernelRegisterExitCallback( sceKernelCreateCallback( HOME_CALLBACK_NAME, ExitCallback, NULL ) );
	return sceKernelSleepThreadCB();	/* コールバックの処理をしないと駄目なのでCBを付きのを使う */
}

//! Homeキーによる終了コールバックの設定を行う
/*!
	@return	正常終了時、0を返す。\n
			設定に失敗した場合は、負数のエラーコードを返す。
*/
int
Cat_SetupCallbacks( void )
{
	return sceKernelStartThread( sceKernelCreateThread( HOME_CALLBACK_THREAD_NAME, HomeKeyCallbackThread, HOME_CALLBACK_THREAD_PRIORITY, HOME_CALLBACK_THREAD_STACK_SIZE, THREAD_ATTR_USER, 0 ), 0, 0 );
}
