//! @file	icTexturePool.h
// テクスチャ管理

#ifndef INCL_CLASS_icTexturePool
#define INCL_CLASS_icTexturePool

#include "icCore.h"

namespace ic {

//! テクスチャ作成者
class icTextureCreator;

//! テクスチャプール
class icTexturePool {
public:
	typedef std::vector<icTexture*> Texture;
	typedef std::vector<icTexture*>::iterator TextureIt;
	typedef std::vector<icTextureCreator*> TextureCreator;
	typedef std::vector<icTextureCreator*>::iterator TextureCreatorIt;

	//! テクスチャ作成者を登録する
	/*!
		@param[in]	pCreator	登録するテクスチャ作成者
	*/
	static void RegisterCreator( icTextureCreator* pCreator );

	//! テクスチャを取得する
	Texture& GetTexture( void );

public:
	//! 作成フラグ
	enum enumCreateFlag {
		eCREATE_FLAG_ALL,				/*!< 全てのテクスチャを作成	*/
		eCREATE_FLAG_THUMB_ONLY,		/*!< サムネイルのみ作成		*/
	};

	//! 作成する
	/*!
		@param[in]	pStream	ストリーム
		@param[in]	eCreateFlag
		@return 正常終了時 true \n
				失敗時 false
	*/
	bool Create( Cat_Stream* pStream, enumCreateFlag eCreateFlag = eCREATE_FLAG_ALL );

	//! 解放する
	void Release( void );

	//! 定義されているテクスチャ数を返す
	/*!
		@return 定義されているテクスチャ数
	*/
	uint32_t	GetTextureCount( void ) const;

	//! インデックスからテクスチャを返す
	/*!
		@return テクスチャ \n
				見つからなかったら0を返す
	*/
	icTexture* SearchFromIndex( uint32_t nIndex );

	//! テクスチャを返す
	/*!
		@param[in]	nGroupNo	グループ番号
		@param[in]	nItemNo		グループ内番号
		@return テクスチャ \n
				見つからなかったら0を返す
	*/
	icTexture* Search( uint16_t nGroupNo, uint16_t nItemNo );
private:
	Texture					m_pTexture;			/*!< テクスチャ			*/
	static TextureCreator	m_TextureCreator;	/*!< テクスチャ作成者	*/
};

//! テクスチャ作成者
class icTextureCreator {
public:
	//! デストラクタ
	virtual ~icTextureCreator() {}

	//! 対応している形式かどうかを調べる
	/*!
		@return	対応している形式の場合true \n
				非対応な場合は、falseを返す
	*/
	virtual bool Check( Cat_Stream* pStream ) = 0;

	//! 作成する
	/*!
		@param[in]	pTexturePool		テクスチャプール
		@param[in]	pStream				ストリーム
		@param[in]	eCreateFlag			作成フラグ
		@return 正常終了時 true \n
				失敗時 false
	*/
	virtual bool Create( icTexturePool* pTexturePool, Cat_Stream* pStream, icTexturePool::enumCreateFlag eCreateFlag ) = 0;
};


} // namespace ic

#endif // INCL_CLASS_icTexturePool
