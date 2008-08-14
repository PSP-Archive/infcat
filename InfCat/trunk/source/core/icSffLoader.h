//! @file	icSffLoader.h

#ifndef INCL_CLASS_icTextureCreatorSff
#define INCL_CLASS_icTextureCreatorSff

#include "icTexturePool.h"

namespace ic {

//! Sffファイル形式からテクスチャを作成する
class icTextureCreatorSff : public icTextureCreator {
public:
	//! コンストラクタ
	icTextureCreatorSff();

	//! 対応している形式かどうかを調べる
	/*!
		@return	対応している形式の場合true \n
				非対応な場合は、falseを返す
	*/
	virtual bool Check( Cat_Stream* pStream );

	//! 作成する
	/*!
		@param[in]	pTexturePool	テクスチャプール
		@param[in]	pStream			ストリーム
		@param[in]	eCreateFlag		作成フラグ
		@return 正常終了時 true \n
				失敗時 false
	*/
	virtual bool Create( icTexturePool* pTexturePool, Cat_Stream* pStream, icTexturePool::enumCreateFlag eCreateFlag );

	//! パレットを設定する
	/*!
		@param[in]	pTexturePool	テクスチャプール
		@param[in]	pPalette		設定するパレット
	*/
	virtual void SetAct( icTexturePool* pTexturePool, Cat_Palette* pPalette );
};

} // namespace ic

#endif // INCL_CLASS_icTextureCreatorSff
