//! @file	Cat_CritSec.h
// クリティカルセクション関連

#ifndef INCL_Cat_CritSec_h
#define INCL_Cat_CritSec_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! クリティカルセクションの構造体
typedef struct {
	void*	pvPrivate;		/*!< 内部データ	*/
} Cat_CritSec;

//! クリティカルセクションの作成
/*!
	@param[in]	fInitState	作成するクリティカルセクションの初期状態を指定する \n
							0以外でロック状態。 0でアンロック状態で初期化する。
	@return	作成したクリティカルセクション \n
			失敗した場合は0が返る。
	@see Cat_CritSecRelease()
	@code
		Cat_CritSec* pCritSec = Cat_CritSecCreate( 0 );
		...
		Cat_CritSecEnter( pCritSec ); {
			// クリティカルセクション
			...
		} Cat_CritSecLeave( pCritSec );
		...
		Cat_CritSecRelease( pCritSec );
	@codeend
*/
extern Cat_CritSec* Cat_CritSecCreate( int32_t fInitState );

//! クリティカルセクションに入る
/*!
	既にクリティカルセクションに入っていた場合に、呼び出すと \n
	ネストし、同じ回数だけCat_CritSecLeave()を呼び出さないと \n
	クリティカルセクションから出ない。
	@param[in]	クリティカルセクション
	@return	成功した場合は正数。失敗した場合は、負数が返る。
	@see Cat_CritSecLeave()
*/
extern int32_t Cat_CritSecEnter( Cat_CritSec* pCritSec );

//! クリティカルセクションから出る
/*!
	@param[in]	クリティカルセクション
	@return	成功した場合は正数。失敗した場合は、負数が返る。
	@see Cat_CritSecEnter()
*/
extern int32_t Cat_CritSecLeave( Cat_CritSec* pCritSec );

//! クリティカルセクションの解放
/*!
	@param[in]	クリティカルセクション
*/
extern void Cat_CritSecRelease( Cat_CritSec* pCritSec );

#ifdef __cplusplus
}
#endif

#endif // INCL_Cat_CritSec_h
