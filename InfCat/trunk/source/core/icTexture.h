//! @file	icTexture.cpp
// テクスチャ

#ifndef INCL_CLASS_icTexture
#define INCL_CLASS_icTexture

#include "icCore.h"

namespace ic {

//! テクスチャクラス
class icTexture {
public:
	//! コンストラクタ
	/*!
		@param[in]	pTexture		テクスチャ
		@param[in]	nGroupNo		グループ番号
		@param[in]	nItemNo			グループ内番号
		@param[in]	nDrawOffsetX	表示オフセットX(ドット単位)
		@param[in]	nDrawOffsetY	表示オフセットY(ドット単位)
	*/
	icTexture( Cat_Texture* pTexture, uint16_t nGroupNo, uint16_t nItemNo, int16_t nDrawOffsetX, int16_t nDrawOffsetY );

	//! コンストラクタ
	/*!
		@param[in]	pTexture		テクスチャ
		@param[in]	nGroupNo		グループ番号
		@param[in]	nItemNo			グループ内番号
		@param[in]	nDrawOffsetX	表示オフセットX(ドット単位)
		@param[in]	nDrawOffsetY	表示オフセットY(ドット単位)
	*/
	icTexture( icTexture* pTexture, uint16_t nGroupNo, uint16_t nItemNo, int16_t nDrawOffsetX, int16_t nDrawOffsetY );

	//! グループ番号の取得
	/*!
		@return グループ番号
	*/
	uint16_t GetGroupNo( void ) const;

	//! グループ内番号の取得
	/*!
		@return グループ内番号
	*/
	uint16_t GetItemNo( void ) const;

	//! 表示オフセットXの取得
	/*!
		@return 表示オフセットX
	*/
	int16_t GetDrawOffsetX( void ) const;

	//! 表示オフセットYの取得
	/*!
		@return 表示オフセットY
	*/
	int16_t GetDrawOffsetY( void ) const;

	//! テクスチャを設定する
	void SetTexture( void );

	//! テクスチャの横幅を取得する
	/*!
		@return	テクスチャの横幅
	*/
	uint32_t GetWidth( void ) const;

	//! テクスチャの高さを取得する
	/*!
		@return	テクスチャの高さ
	*/
	uint32_t GetHeight( void ) const;

	//! テクスチャの実際の横幅を取得する
	/*!
		内部で加工された後のテクスチャのサイズ
		@return	テクスチャの実際の横幅
	*/
	uint32_t GetRealWidth( void ) const;

	//! テクスチャの実際の高さを取得する
	/*!
		内部で加工された後のテクスチャのサイズ
		@return	テクスチャの実際の高さ
	*/
	uint32_t GetRealHeight( void ) const;

	//! テクスチャのパレットを取得する
	/*!
		@return	テクスチャのパレット
	*/
	Cat_Palette* GetPalette( void );

	//! テクスチャのパレットを設定する
	/*!
		@param[in]	pPalette	設定するパレット
	*/
	void SetPalette( Cat_Palette* pPalette );

	//! ユーザーデータを取得
	/*!
		@return ユーザーデータ
	*/
	void* GetUserData( void );

	//! ユーザーデータを設定
	/*!
		@param[in]	pvUserData	ユーザーデータ
	*/
	void SetUserData( void* pvUserData );

private:
	boost::shared_ptr<class icTextureImpl>	m_impl;		/*!< 実装	*/
};

//! 演算子 <
bool operator<( icTexture& a, icTexture& b );

//! 演算子 ==
bool operator==( icTexture& a, icTexture& b );

//! 演算子 !=
bool operator!=( icTexture& a, icTexture& b );

} // namespace ic

#endif	// INCL_CLASS_icTexture
