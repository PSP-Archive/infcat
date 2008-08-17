//! @file	icDef.h
// キャラクタ定義ファイル

#ifndef INCL_CLASS_icDef
#define INCL_CLASS_icDef

#include "icCore.h"

namespace ic {

//! キャラクタ定義ファイル
class icDef {
public:
	//! キャラクタ定義ファイルを読み込む
	/*!
		@param[in]	pStream	ストリーム
		@return 成功した場合true \n
				失敗したらfalseを返す
	*/
	bool Load( Cat_Stream* pStream );

	//! 値の項目が幾つあるか
	/*!
		@param[in]	strSection	セクション
		@param[in]	strKey		キー
		@return 値の項目数
	*/
	int32_t Count( const std::string& strSection, const std::string& strKey );

	//! 値を取得する
	/*!
		@param[in]	strSection	セクション
		@param[in]	strKey		キー
		@param[in]	nIndex		値のインデックス
		@return 値
	*/
	std::string GetValue( const std::string& strSection, const std::string& strKey, int32_t nIndex );
private:
	icSectionValue	m_SectionValue;		/*!< 項目 */
};

} // namespace ic

#endif // INCL_CLASS_icDef
