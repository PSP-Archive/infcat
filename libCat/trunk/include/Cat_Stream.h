//! @file	Cat_Stream.h
// ストリーム関連

#ifndef INC_Cat_Stream_h
#define INC_Cat_Stream_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! ストリーム構造体
typedef struct _Cat_Stream {
	//! 読み込む
	/*!
		@param[in]	pStream	ストリーム
		@param[out]	pvData	読み込みバッファ
		@param[in]	nSize	読み込むサイズ(バイト単位)
		@reutrn	読み込んだサイズ。負の場合はエラー
	*/
	int64_t (*Read)( struct _Cat_Stream* pStream, void* pvData, int64_t nSize );

	//! 書き込む
	/*!
		@param[in]	pStream	ストリーム
		@param[in]	pvData	書き込むバッファ
		@param[in]	nSize	書き込むサイズ(バイト単位)
		@reutrn	書き込んだサイズ。負の場合はエラー
	*/
	int64_t (*Write)( struct _Cat_Stream* pStream, const void* pvData, int64_t nSize );

	//! 位置を設定する
	int64_t (*Seek)( struct _Cat_Stream* pStream, int64_t nOffset );

	//! 閉じる
	void (*Close)( struct _Cat_Stream* pStream );

	//! 位置を取得する
	/*!
		@param[in]	pStream
		@reutrn	位置
	*/
	int64_t (*Tell)( struct _Cat_Stream* pStream );

	//! サイズを取得する
	/*!
		@param[in]	pStream
		@reutrn	サイズ(バイト単位)
	*/
	int64_t (*GetSize)( struct _Cat_Stream* pStream );

	void* pvPrivate;	/*!< 内部データ */
} Cat_Stream;

//! 読み込む
/*!
	@param[in]	pStream	ストリーム
	@param[out]	pvData	読み込みバッファ
	@param[in]	nSize	読み込むサイズ(バイト単位)
	@reutrn	読み込んだサイズ。負の場合はエラー
*/
extern int64_t Cat_StreamRead( Cat_Stream* pStream, void* pvData, int64_t nSize );

//! 書き込む
/*!
	@param[in]	pStream	ストリーム
	@param[in]	pvData	書き込むバッファ
	@param[in]	nSize	書き込むサイズ(バイト単位)
	@reutrn	書き込んだサイズ。負の場合はエラー
*/
extern int64_t Cat_StreamWrite( Cat_Stream* pStream, const void* pvData, int64_t nSize );

//! 閉じる
/*!
	@param[in]	pStream
*/
extern void Cat_StreamClose( Cat_Stream* pStream );

//! 位置を取得する
/*!
	@param[in]	pStream
	@reutrn	位置。失敗したら負数を返す
*/
extern int64_t Cat_StreamTell( Cat_Stream* pStream );

//! 位置を設定する
/*!
	@param[in]	pStream
	@param[in]	nOffset	設定する位置
	@reutrn	失敗したら負数を返す
*/
extern int64_t Cat_StreamSeek( Cat_Stream* pStream, int64_t nOffset );

//! サイズを取得する
/*!
	@param[in]	pStream
	@reutrn	サイズ(バイト単位) \n
			ファイルサイズが不明、もしくは、失敗した場合は負数を返す
*/
extern int64_t Cat_StreamGetSize( Cat_Stream* pStream );

#ifdef __cplusplus
}
#endif

#endif // INC_Cat_Stream_h
