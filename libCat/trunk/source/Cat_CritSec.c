//! @file	Cat_CritSec.c
// クリティカルセクション関連

#include <pspthreadman.h>
#include <malloc.h>	// for memalign
#include "Cat_CritSec.h"

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

//! 排他用
static SceUID	gCritSecMutex = -1;

//! クリティカルセクションの構造体
typedef struct {
	SceUID	sema;				/*!< クリティカルセクション用					*/
	SceUID	nEnterThreadID;		/*!< クリティカルセクションに入ってるスレッド	*/
	int32_t	nRef;				/*!< ネスト数									*/
} Cat_CritSec_Priv;

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
Cat_CritSec* Cat_CritSecCreate( int32_t fInitState )
{
	Cat_CritSec_Priv* rc;

	if(gCritSecMutex < 0) {
		gCritSecMutex = sceKernelCreateSema( "gm", 0, 0, 1, 0 );
		if(gCritSecMutex < 0) {
			return 0;
		}
	}

	rc = (Cat_CritSec_Priv*)CAT_MALLOC( sizeof(Cat_CritSec_Priv) );
	if(rc) {
		if(fInitState) {
			// ロック状態で初期化
			rc->sema = sceKernelCreateSema( "s", 0, 0, 1, 0 );
			rc->nEnterThreadID = sceKernelGetThreadId();
			rc->nRef = 1;
		} else {
			// アンロック状態で初期化
			rc->sema = sceKernelCreateSema( "s", 0, 1, 1, 0 );
			rc->nEnterThreadID = -1;
			rc->nRef = 0;
		}
		if(rc->sema < 0) {
			CAT_FREE( rc );
			rc = 0;
		}
	}

	sceKernelSignalSema( gCritSecMutex, 1 );
	return (Cat_CritSec*)rc;
}

//! クリティカルセクションに入る
/*!
	既にクリティカルセクションに入っていた場合に、呼び出すと \n
	ネストし、同じ回数だけCat_CritSecLeave()を呼び出さないと \n
	クリティカルセクションから出ない。
	@param[in]	クリティカルセクション
	@return	成功した場合は正数。失敗した場合は、負数が返る。
	@see Cat_CritSecLeave()
*/
int32_t
Cat_CritSecEnter( Cat_CritSec* pCritSec )
{
	volatile Cat_CritSec_Priv* pPrivate = (Cat_CritSec_Priv*)pCritSec;
	int32_t rc;

	if(gCritSecMutex < 0) {
		return -1;
	}
	if(pPrivate == 0) {
		// 引数が変
		return -1;
	}

	rc = sceKernelWaitSema( gCritSecMutex, 1, 0 );
	if(rc >= 0) {
		if(pPrivate->nEnterThreadID != sceKernelGetThreadId()) {
			// 他のスレッドが入ってるので、出るまで待つ
			volatile SceUID sema = pPrivate->sema;
			sceKernelSignalSema( gCritSecMutex, 1 );

			rc = sceKernelWaitSemaCB( sema, 1, 0 );

			if(rc >= 0) {
				rc = sceKernelWaitSema( gCritSecMutex, 1, 0 );
				if(rc >= 0) {
					// 情報更新
					pPrivate->nEnterThreadID = sceKernelGetThreadId();
					pPrivate->nRef = 1;
					sceKernelSignalSema( gCritSecMutex, 1 );
				}
			}
		} else {
			// 既にクリティカルセクションに入っているので、参照数を加算しておく
			pPrivate->nRef++;
			sceKernelSignalSema( gCritSecMutex, 1 );
		}
	}

	return rc;
}

//! クリティカルセクションから出る
/*!
	@param[in]	クリティカルセクション
	@return	成功した場合は正数。失敗した場合は、負数が返る。
	@see Cat_CritSecEnter()
*/
int32_t
Cat_CritSecLeave( Cat_CritSec* pCritSec )
{
	volatile Cat_CritSec_Priv* pPrivate = (Cat_CritSec_Priv*)pCritSec;
	int32_t rc;

	if(gCritSecMutex < 0) {
		return -1;
	}
	if(pPrivate == 0) {
		// 引数が変
		return -1;
	}

	rc = sceKernelWaitSema( gCritSecMutex, 1, 0 );
	if(rc >= 0) {
		if(pPrivate->nEnterThreadID == sceKernelGetThreadId()) {
			if(pPrivate->nRef == 1) {
				// 解除
				pPrivate->nEnterThreadID = -1;
				pPrivate->nRef = 0;
				sceKernelSignalSema( pPrivate->sema, 1 );
			} else {
				pPrivate->nRef--;
			}
		} else {
			// 自分は入ってないのに、出ようとした
			rc = -1;	// エラー
		}

		sceKernelSignalSema( gCritSecMutex, 1 );
	}

	return rc;
}

//! クリティカルセクションの解放
/*!
	@param[in]	クリティカルセクション
*/
void
Cat_CritSecRelease( Cat_CritSec* pCritSec )
{
	volatile Cat_CritSec_Priv* pPrivate = (Cat_CritSec_Priv*)pCritSec;
	int32_t rc;

	if(gCritSecMutex < 0) {
		return;
	}
	if(pPrivate == 0) {
		// 引数が変
		return;
	}

	rc = sceKernelWaitSema( gCritSecMutex, 1, 0 );
	if(rc >= 0) {
		pPrivate->nEnterThreadID = -1;
		pPrivate->nRef = 0;
		if(pPrivate->sema >= 0) {
			volatile SceUID sema = pPrivate->sema;
			pPrivate->sema = -1;
			sceKernelDeleteSema( sema );
		}
		CAT_FREE( pCritSec );

		sceKernelSignalSema( gCritSecMutex, 1 );
	}
}
