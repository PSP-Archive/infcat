//! @file	icTexturePool.cpp
// テクスチャ管理

#include "icCore.h"

namespace ic {

icTexturePool::TextureCreator	icTexturePool::m_TextureCreator;	/*!< テクスチャ作成者	*/

//! テクスチャ作成者を登録する
/*!
	@param[in]	pCreator	登録するテクスチャ作成者
*/
void
icTexturePool::RegisterCreator( icTextureCreator* pCreator )
{
	m_TextureCreator.push_back( pCreator );
}

//! テクスチャを取得する
icTexturePool::Texture&
icTexturePool::GetTexture( void )
{
	return m_pTexture;
}

//! 作成する
/*!
	@param[in]	pStream	ストリーム
	@param[in]	eCreateFlag
	@return 正常終了時 true \n
			失敗時 false
*/
bool
icTexturePool::Create( Cat_Stream* pStream, enumCreateFlag eCreateFlag )
{
	for(TextureCreatorIt p = m_TextureCreator.begin(); p != m_TextureCreator.end(); p++) {
		if((*p)->Check( pStream )) {
			return (*p)->Create( this, pStream, eCreateFlag );
		}
	}
	return false;
}

//! 解放する
void
icTexturePool::Release( void )
{
	for(TextureIt p = m_pTexture.begin(); p != m_pTexture.end(); p++) {
		delete *p;
	}
	m_pTexture.clear();
}

//! 定義されているテクスチャ数を返す
/*!
	@return 定義されているテクスチャ数
*/
uint32_t
icTexturePool::GetTextureCount( void ) const
{
	return m_pTexture.size();
}

//! インデックスからテクスチャを返す
/*!
	@return テクスチャ \n
			見つからなかったら0を返す
*/
icTexture*
icTexturePool::SearchFromIndex( uint32_t nIndex )
{
	if((nIndex < 0) || (nIndex >= GetTextureCount())) {
		return 0;
	}
	return m_pTexture[nIndex];
}

//! テクスチャを返す
/*!
	@param[in]	nGroupNo	グループ番号
	@param[in]	nItemNo		グループ内番号
	@return テクスチャ \n
			見つからなかったら0を返す
*/
icTexture*
icTexturePool::Search( uint16_t nGroupNo, uint16_t nItemNo )
{
	for(TextureIt p = m_pTexture.begin(); p != m_pTexture.end(); p++) {
		if(((*p)->GetGroupNo() == nGroupNo) && ((*p)->GetItemNo() == nItemNo)) {
			return (*p);
		}
	}
	return 0;
}

} // namespace ic
