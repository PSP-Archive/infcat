//! @file	icSffLoader.h

#ifndef INCL_CLASS_icSff
#define INCL_CLASS_icSff

#include "icCore.h"

namespace ic {

class icTexture;

class icSff {
	typedef std::vector<icTexture*> Texture;
	typedef std::vector<icTexture*>::iterator TextureIt;

	Texture		m_pTexture;		/*!< テクスチャ				*/
public:
	//! デストラクタ
	~icSff();

	//! 作成フラグ
	enum enumFlag {
		eCREATE_ALL,			/*!< 全てのテクスチャを作成	*/
		eCREATE_THUMB_ONLY,		/*!< サムネイルのみ作成		*/
	};

	//! 作成する
	/*!
		@param[in]	pStream	ストリーム
		@param[in]	eFlag
		@return 正常終了時 true \n
				失敗時 false
	*/
	bool Create( Cat_Stream* pStream, enumFlag eFlag = eCREATE_ALL );

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
};

} // namespace ic

#endif // INCL_CLASS_icSff
