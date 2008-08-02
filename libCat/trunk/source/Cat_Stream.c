//! @file	Cat_Stream.c
// ストリーム関連

#include "Cat_Stream.h"

//! 読み込む
/*!
	@param[in]	pStream	ストリーム
	@param[out]	pvData	読み込みバッファ
	@param[in]	nSize	読み込むサイズ(バイト単位)
	@reutrn	読み込んだサイズ。負の場合はエラー
*/
int64_t
Cat_StreamRead( Cat_Stream* pStream, void* pvData, int64_t nSize )
{
	if(pStream && pStream->Read && pvData && (nSize >= 0)) {
		return pStream->Read( pStream, pvData, nSize );
	}
	return -1;
}

//! 書き込む
/*!
	@param[in]	pStream	ストリーム
	@param[in]	pvData	書き込むバッファ
	@param[in]	nSize	書き込むサイズ(バイト単位)
	@reutrn	書き込んだサイズ。負の場合はエラー
*/
int64_t
Cat_StreamWrite( Cat_Stream* pStream, const void* pvData, int64_t nSize )
{
	if(pStream && pStream->Read && pvData && (nSize >= 0)) {
		return pStream->Write( pStream, pvData, nSize );
	}
	return -1;
}

//! 閉じる
/*!
	@param[in]	pStream
*/
void
Cat_StreamClose( Cat_Stream* pStream )
{
	if(pStream && pStream->Close) {
		pStream->Close( pStream );
	}
}

//! 位置を取得する
/*!
	@param[in]	pStream
	@reutrn	位置。失敗したら負数を返す
*/
int64_t
Cat_StreamTell( Cat_Stream* pStream )
{
	if(pStream && pStream->Tell) {
		return pStream->Tell( pStream );
	}
	return -1;
}

//! 位置を設定する
/*!
	@param[in]	pStream
	@param[in]	nOffset	設定する位置
	@reutrn	失敗したら負数を返す
*/
int64_t
Cat_StreamSeek( Cat_Stream* pStream, int64_t nOffset )
{
	if(pStream && pStream->Seek) {
		return pStream->Seek( pStream, nOffset );
	}
	return -1;
}

//! サイズを取得する
/*!
	@param[in]	pStream
	@reutrn	サイズ(バイト単位) \n
			ファイルサイズが不明、もしくは、失敗した場合は負数を返す
*/
int64_t
Cat_StreamGetSize( Cat_Stream* pStream )
{
	if(pStream && pStream->GetSize) {
		return pStream->GetSize( pStream );
	}
	return -1;
}
