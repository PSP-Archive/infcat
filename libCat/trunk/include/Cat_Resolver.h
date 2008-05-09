//! @file Cat_Resolver.h
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

#ifndef INCL_Cat_Resolver_h
#define INCL_Cat_Resolver_h

#if defined(__cplusplus)
extern "C" {
#endif

struct in_addr;

//! リゾルバエンジンの初期化
/*!
	@return	正常終了時、0。\n
			初期化に失敗した場合は、負数のエラーコードが返る。
	@see	Cat_ResolverTermEngine()
*/
extern int Cat_ResolverInitEngine( void );

//! リゾルバエンジンの終了処理
/*!
	@see	Cat_ResolverInitEngine();
*/
extern void Cat_ResolverTermEngine( void );

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
extern int Cat_ResolverURL( const char* pszURL, struct in_addr* pDest );

#if defined(__cplusplus)
};
#endif

#endif
