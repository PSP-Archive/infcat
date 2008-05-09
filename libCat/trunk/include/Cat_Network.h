//! @file Cat_Network.h
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

#ifndef INCL_Cat_Network_h
#define INCL_Cat_Network_h

#ifdef __cplusplus
extern "C" {
#endif

//! ネットワークの初期化
/*!
	ネットワークモジュールをあらかじめ読み込んでおく必要がある。
	@return	正常終了時、0。\n
			初期化に失敗した場合は、負数のエラーコードが返る。
*/
extern int Cat_NetworkInit( void );

//! ネットワークの切断と終了処理
/*!
	@see	Cat_NetworkInit()
*/
extern void Cat_NetworkTerm( void );

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
extern int Cat_NetworkConnect( void (*draw_callback)(void*), void (*screen_update_callback)(void*), void* pvUserData );

//! ネットワーク接続状態を取得
/*!
	@return	接続している状態なら1。そうでないなら0を返す。
*/
extern int Cat_NetworkIsConnect( void );

#ifdef __cplusplus
}
#endif

#endif
