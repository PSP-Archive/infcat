//! @file Cat_Resolver.c
// 名前解決

/*
	// 初期化
	Cat_ResolverInitEngine();
	...
	...
	// URLの名前を解決する
	if(Cat_ResolverURL( "neko.mimi.com", &addr ) < 0) {
		// エラー
	}
	...
	...
	// 終了処理
	Cat_ResolverTermEngine();
*/

#include <pspthreadman.h>
#include <pspnet_resolver.h>
#include <arpa/inet.h>	/* for inet_aton */
#include <string.h>
#include <stdlib.h>
#include <malloc.h>	// for memalign
#include "Cat_Resolver.h"

//! リゾルバエンジンのワークバッファサイズ
#define RESOLVER_WORK_BUFFER_SIZE (2*1024)

//! URLキャッシュ数
#define MAX_URL_CACHE (256)

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

#ifdef DEBUG_TRACE
#define TRACE(x) pspDebugScreenPrintf x
#else
#define TRACE(x)
#endif

//! アドレスセット
/*!
	URLキャッシュで使用している
*/
typedef struct {
	struct in_addr address;		/*!< アドレス	*/
	char* pszURL;				/*!< URL		*/
} AddrSet;

//! URLキャッシュ
static AddrSet AddressSet[MAX_URL_CACHE];
//! URLキャッシュ用インデックス
static int m_nRIndex = 0;
//! 排他用のセマフォ
static SceUID m_semaResolver = -1;

// リゾルバ
int resolve_host( struct in_addr* sin_addr, const char* hostname );

//! リゾルバエンジンの初期化
/*!
	@return	正常終了時、0。\n
			初期化に失敗した場合は、負数のエラーコードが返る。
	@see	Cat_ResolverTermEngine()
*/
int
Cat_ResolverInitEngine( void )
{
	int rc;

	Cat_ResolverTermEngine();

	memset( AddressSet, 0, sizeof(AddressSet) );
	m_nRIndex = 0;
	rc = m_semaResolver = sceKernelCreateSema( "semaResolve", 0, 1, 1, 0 );
	if(rc >= 0) {
		rc = sceNetResolverInit();
		if(rc < 0) {
			sceKernelDeleteSema( m_semaResolver );
			m_semaResolver = -1;
		}
	}

	return rc;
}

//! リゾルバエンジンの終了処理
/*!
	@see	Cat_ResolverInitEngine();
*/
void
Cat_ResolverTermEngine( void )
{
	if(m_semaResolver >= 0) {
		int i;
		sceKernelWaitSemaCB( m_semaResolver, 1, 0 );	/* 解決中のものがあれば、終了するのを待つ */

		/* キャッシュを解放 */
		for(i = 0; i < MAX_URL_CACHE; i++) {
			if(AddressSet[i].pszURL) {
				CAT_FREE( AddressSet[i].pszURL );
			}
		}
		memset( AddressSet, 0, sizeof(AddressSet) );
		m_nRIndex = 0;

		sceKernelDeleteSema( m_semaResolver );
		m_semaResolver = -1;

		sceNetResolverTerm();
	}
}

//! 名前解決を行う
/*!
	\a pszURL で指定されたURLの名前を解決する。 \n
	この関数を使用する前に、Cat_ResolverInitEngine()で初期化をしておく必要がある。
	@code
		// 初期化
		Cat_ResolverInitEngine();
		...
		...
		// URLの名前を解決する
		if(Cat_ResolverURL( "neko.mimi.com", &addr ) < 0) {
			// エラー
		}
		...
		...
		// 終了処理
		Cat_ResolverTermEngine();
	@endcode

	@param[in]	pszURL	名前解決を行うURL
	@param[out]	pDest	結果が格納される
	@return	正常終了時、0。\n
			失敗した場合は、負数のエラーコードが返る。
	@see	Cat_ResolverInitEngine();
	@see	Cat_ResolverTermEngine()
*/
int
Cat_ResolverURL( const char* pszURL, struct in_addr* pDest )
{
	int i;
	int rc;

	if(pDest == 0 || pszURL == 0 || m_semaResolver < 0) {
		return -1;
	}

	if(sceKernelWaitSemaCB( m_semaResolver, 1, 0 ) < 0) {
		return -1;
	}

	rc = -1;
	for(i = 0; i < MAX_URL_CACHE; i++) {
		if(AddressSet[i].pszURL && (strcmp( AddressSet[i].pszURL, pszURL ) == 0)) {
			/* 発見した */
			memcpy( pDest, &AddressSet[i].address, sizeof(struct in_addr) );
			rc = 0;
			break;
		}
	}
	if(i >= MAX_URL_CACHE) {
		/* 見つからなかったら、適当に */
		AddrSet* pReprace = &AddressSet[m_nRIndex];
		m_nRIndex = (m_nRIndex + 1) % MAX_URL_CACHE;

		if(pReprace->pszURL) {
			CAT_FREE( pReprace->pszURL );
		}
		pReprace->pszURL = (char*)CAT_MALLOC( strlen( pszURL ) + 1 );
		if(pReprace->pszURL) {
			strcpy( pReprace->pszURL, pszURL );
		}
		memset( &pReprace->address, 0, sizeof(struct in_addr) );

		rc = resolve_host( &pReprace->address, pszURL );
		if(rc >= 0) {
			memcpy( pDest, &pReprace->address, sizeof(struct in_addr) );
			rc = 0;
		} else {
			if(pReprace->pszURL) {
				CAT_FREE( pReprace->pszURL );
			}
			pReprace->pszURL = 0;
			memset( &pReprace->address, 0, sizeof(struct in_addr) );
		}
	}

	sceKernelSignalSema( m_semaResolver, 1 );
	return rc;
}

//! リゾルバ
/*!
	名前解決を行う。\n
	排他処理をしないので、スレッドセーフではないので注意。\n
	キャッシュ制御もしない。
	@param[out]	sin_addr
	@param[in]	hostname
	@return		正常終了時、0。\n
				失敗した場合は、負数のエラーコードを返す。
*/
int
resolve_host( struct in_addr* sin_addr, const char* hostname )
{
	int rid = -1;
	char* buf;
	int rc;

	/* 引数チェック */
	if(sin_addr == 0 || hostname == 0) {
		return -1;
	}

	if(('0' <= hostname[0]) && (hostname[0] <= '9')) {
		/* xxx.xxx.xxx.xxx形式かな */
		if(inet_aton( hostname, sin_addr ) != 0) {
			/* 正常に変換できた */
			return 0;
		}
	}

	buf = (char*)CAT_MALLOC( RESOLVER_WORK_BUFFER_SIZE );

	do {
		/* Create a resolver */
		rc = sceNetResolverCreate( &rid, buf, RESOLVER_WORK_BUFFER_SIZE );
		if(rc < 0) {
			TRACE(( "Error creating resolver:error code %08X[%s]\n", rc, hostname ));
			break;
		}

		/* Resolve a name to an ip address */
		rc = sceNetResolverStartNtoA( rid, hostname, sin_addr, 5 * 1000 * 1000, 5 );
		if(rc < 0) {
			TRACE(( "Error sceNetResolverStartNtoA:error code %08X[%s]\n", rc, hostname ));
			break;
		}
	} while(0);

	if(rid >= 0) {
		sceNetResolverDelete( rid );
	}

	if(buf) {
		CAT_FREE( buf );
	}

    return rc;
}
