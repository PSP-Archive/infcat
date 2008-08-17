//! @file	icTextReader.h
// テキストの読み込み

#ifndef INC_CLASS_icTextReader
#define INC_CLASS_icTextReader

#include "icCore.h"

namespace ic {

//! テキストの読み込み
class icTextReader {
public:
	//! コンストラクタ
	/*!
		@param[in]	pStream	ストリーム
		@param[in]	fClose	終了時にストリームを閉じるかどうか
	*/
	icTextReader( Cat_Stream* pStream, bool fClose = false );

	//! １行読み込む
	/*!
		@return	改行コードを含まない文字列
	*/
	std::string ReadLine( void );

	//! 行番号を取得する
	uint32_t GetLineNo( void ) const;

	//! 位置を取得する
	uint32_t GetOffset( void ) const;

	//! 終了したかどうか
	/*!
		@return 終了してたらtrue \n
				してなかったらfalseを返す
	*/
	bool eof( void );

private:
	boost::shared_ptr<class icTextReaderImpl>	m_impl;		/*!< 実装	*/
};

} // namespace ic

#endif // INC_CLASS_icTextReader
