//! @file	icTexture.cpp
// テクスチャ

#include "icCore.h"

namespace ic {

//! 実装
class icTextureImpl {
public:
	//! コンストラクタ
	/*!
		@param[in]	pTexture		テクスチャ
		@param[in]	nGroupNo		グループ番号
		@param[in]	nItemNo			グループ内番号
		@param[in]	nDrawOffsetX	表示オフセットX(ドット単位)
		@param[in]	nDrawOffsetY	表示オフセットY(ドット単位)
	*/
	icTextureImpl( Cat_Texture* pTexture, uint16_t nGroupNo, uint16_t nItemNo, int16_t nDrawOffsetX, int16_t nDrawOffsetY )
		: m_pTexture( pTexture )
		, m_nGroupNo( nGroupNo )
		, m_nItemNo( nItemNo )
		, m_nDrawOffsetX( nDrawOffsetX )
		, m_nDrawOffsetY( nDrawOffsetY )
	{
		if(m_pTexture) {
			Cat_TextureAddRef( m_pTexture );	// 参照カウントを加算しとく
		}
	}

	//! コンストラクタ
	/*!
		@param[in]	pTexture		テクスチャ
		@param[in]	nGroupNo		グループ番号
		@param[in]	nItemNo			グループ内番号
		@param[in]	nDrawOffsetX	表示オフセットX(ドット単位)
		@param[in]	nDrawOffsetY	表示オフセットY(ドット単位)
	*/
	icTextureImpl( icTextureImpl* pTexture, uint16_t nGroupNo, uint16_t nItemNo, int16_t nDrawOffsetX, int16_t nDrawOffsetY )
		: m_pTexture( pTexture->m_pTexture )
		, m_nGroupNo( nGroupNo )
		, m_nItemNo( nItemNo )
		, m_nDrawOffsetX( nDrawOffsetX )
		, m_nDrawOffsetY( nDrawOffsetY )
	{
		if(m_pTexture) {
			Cat_TextureAddRef( m_pTexture );	// 参照カウントを加算しとく
		}
	}

	//! デストラクタ
	~icTextureImpl() {
		if(m_pTexture) {
			Cat_TextureRelease( m_pTexture );
			m_pTexture = 0;
		}
	}

	//! グループ番号の取得
	/*!
		@return グループ番号
	*/
	uint16_t GetGroupNo( void ) const {
		return m_nGroupNo;
	}

	//! グループ内番号の取得
	/*!
		@return グループ内番号
	*/
	uint16_t GetItemNo( void ) const {
		return m_nItemNo;
	}

	//! 表示オフセットXの取得
	/*!
		@return 表示オフセットX
	*/
	int16_t GetDrawOffsetX( void ) const {
		return m_nDrawOffsetX;
	}

	//! 表示オフセットYの取得
	/*!
		@return 表示オフセットY
	*/
	int16_t GetDrawOffsetY( void ) const {
		return m_nDrawOffsetY;
	}

	//! テクスチャを設定する
	void SetTexture( void ) {
		Cat_TextureSetTexture( m_pTexture );
	}

	//! テクスチャの横幅を取得する
	/*!
		@return	テクスチャの横幅
	*/
	uint32_t GetWidth( void ) const {
		return Cat_TextureGetWidth( m_pTexture );
	}

	//! テクスチャの高さを取得する
	/*!
		@return	テクスチャの高さ
	*/
	uint32_t GetHeight( void ) const {
		return Cat_TextureGetHeight( m_pTexture );
	}

	//! テクスチャの実際の横幅を取得する
	/*!
		内部で加工された後のテクスチャのサイズ
		@return	テクスチャの実際の横幅
	*/
	uint32_t GetRealWidth( void ) {
		return m_pTexture->nTextureWidth;
	}

	//! テクスチャの実際の高さを取得する
	/*!
		内部で加工された後のテクスチャのサイズ
		@return	テクスチャの実際の高さ
	*/
	uint32_t GetRealHeight( void ) {
		return m_pTexture->nTextureHeight;
	}

	//! テクスチャのパレットを取得する
	/*!
		@return	テクスチャのパレット
	*/
	Cat_Palette* GetPalette( void ) {
		return m_pTexture->pPalette;
	}

	//! テクスチャのパレットを設定する
	/*!
		@param[in]	pPalette	設定するパレット
	*/
	void SetPalette( Cat_Palette* pPalette ) {
		if(pPalette && (m_pTexture->pPalette != pPalette)) {
			Cat_PaletteRelease( m_pTexture->pPalette );
			m_pTexture->pPalette = pPalette;
			Cat_PaletteAddRef( pPalette );
		}
	}
private:
	Cat_Texture*	m_pTexture;			/*!< テクスチャ						*/
	uint16_t		m_nGroupNo;			/*!< グループ番号					*/
	uint16_t		m_nItemNo;			/*!< グループ内番号					*/
	int16_t			m_nDrawOffsetX;		/*!< 表示オフセットX(ドット単位)	*/
	int16_t			m_nDrawOffsetY;		/*!< 表示オフセットY(ドット単位)	*/
};

//! コンストラクタ
/*!
	@param[in]	pTexture		テクスチャ
	@param[in]	nGroupNo		グループ番号
	@param[in]	nItemNo			グループ内番号
	@param[in]	nDrawOffsetX	表示オフセットX(ドット単位)
	@param[in]	nDrawOffsetY	表示オフセットY(ドット単位)
*/
icTexture::icTexture( Cat_Texture* pTexture, uint16_t nGroupNo, uint16_t nItemNo, int16_t nDrawOffsetX, int16_t nDrawOffsetY )
	: m_impl( new icTextureImpl( pTexture, nGroupNo, nItemNo, nDrawOffsetX, nDrawOffsetY ) )
{
}

//! コンストラクタ
/*!
	@param[in]	pTexture		テクスチャ
	@param[in]	nGroupNo		グループ番号
	@param[in]	nItemNo			グループ内番号
	@param[in]	nDrawOffsetX	表示オフセットX(ドット単位)
	@param[in]	nDrawOffsetY	表示オフセットY(ドット単位)
*/
icTexture::icTexture( icTexture* pTexture, uint16_t nGroupNo, uint16_t nItemNo, int16_t nDrawOffsetX, int16_t nDrawOffsetY )
	: m_impl( new icTextureImpl( pTexture->m_impl.get(), nGroupNo, nItemNo, nDrawOffsetX, nDrawOffsetY ) )
{
}

//! グループ番号の取得
/*!
	@return グループ番号
*/
uint16_t
icTexture::GetGroupNo( void ) const
{
	return m_impl->GetGroupNo();
}

//! グループ内番号の取得
/*!
	@return グループ内番号
*/
uint16_t
icTexture::GetItemNo( void ) const
{
	return m_impl->GetItemNo();
}

//! 表示オフセットXの取得
/*!
	@return 表示オフセットX
*/
int16_t
icTexture::GetDrawOffsetX( void ) const
{
	return m_impl->GetDrawOffsetX();
}

//! 表示オフセットYの取得
/*!
	@return 表示オフセットY
*/
int16_t
icTexture::GetDrawOffsetY( void ) const
{
	return m_impl->GetDrawOffsetY();
}

//! テクスチャを設定する
void
icTexture::SetTexture( void )
{
	m_impl->SetTexture();
}

//! テクスチャの横幅を取得する
/*!
	@return	テクスチャの横幅
*/
uint32_t
icTexture::GetWidth( void ) const
{
	return m_impl->GetWidth();
}

//! テクスチャの高さを取得する
/*!
	@return	テクスチャの高さ
*/
uint32_t
icTexture::GetHeight( void ) const
{
	return m_impl->GetHeight();
}

//! テクスチャの実際の横幅を取得する
/*!
	内部で加工された後のテクスチャのサイズ
	@return	テクスチャの実際の横幅
*/
uint32_t
icTexture::GetRealWidth( void ) const
{
	return m_impl->GetRealWidth();
}

//! テクスチャの実際の高さを取得する
/*!
	内部で加工された後のテクスチャのサイズ
	@return	テクスチャの実際の高さ
*/
uint32_t
icTexture::GetRealHeight( void ) const
{
	return m_impl->GetRealHeight();
}

//! テクスチャのパレットを取得する
/*!
	@return	テクスチャのパレット
*/
Cat_Palette*
icTexture::GetPalette( void )
{
	return m_impl->GetPalette();
}

//! テクスチャのパレットを設定する
/*!
	@param[in]	pPalette	設定するパレット
*/
void
icTexture::SetPalette( Cat_Palette* pPalette )
{
	m_impl->SetPalette( pPalette );
}

//! 演算子 <
bool
operator<( icTexture& a, icTexture& b )
{
	if(a.GetGroupNo() != b.GetGroupNo()) {
		return a.GetGroupNo() < b.GetGroupNo();
	} else {
		return a.GetItemNo() < b.GetItemNo();
	}
}

//! 演算子 ==
bool
operator==( icTexture& a, icTexture& b )
{
	return (a.GetGroupNo() == b.GetGroupNo())
		&& (a.GetItemNo() == b.GetItemNo());
}

//! 演算子 !=
bool
operator!=( icTexture& a, icTexture& b )
{
	return !(a == b);
}

} // namespace ic
