//! @file Cat_PspCallback.h
// Homeキーによる終了コールバックの設定を行う

#ifndef INCL_Cat_PspCallback_h
#define INCL_Cat_PspCallback_h

#if defined(__cplusplus)
extern "C" {
#endif

//! Homeキーによる終了コールバックの設定を行う
/*!
	@return	正常終了時、0を返す。\n
			設定に失敗した場合は、負数のエラーコードを返す。
*/
extern int Cat_SetupCallbacks( void );

#if defined(__cplusplus)
};
#endif

#endif // INCL_Cat_PspCallback_h
