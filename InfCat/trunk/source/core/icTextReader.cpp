//! @file	icTextReader.cpp
// テキストの読み込み

#include "icCore.h"

namespace ic {

//! 実装
class icTextReaderImpl {
public:
	//! コンストラクタ
	/*!
		@param[in]	pStream	ストリーム
		@param[in]	fClose	終了時にストリームを閉じるかどうか
	*/
	icTextReaderImpl( Cat_Stream* pStream, bool fClose )
		: m_pStream( pStream )
		, m_nLine( 0 )
		, m_nOffset( 0 )
		, m_fUnget( false )
		, m_nChar( 0 )
		, m_fEof( pStream ? false : true )
		, m_fClose( fClose )
	{
	}

	//! デストラクタ
	~icTextReaderImpl() {
		if(m_fClose) {
			Cat_StreamClose( m_pStream );
			m_pStream = 0;
		}
	}

	//! １行読み込む
	/*!
		@return	改行コードを含まない文字列
	*/
	std::string ReadLine( void ) {
		std::string	strLine;
		int32_t ch;

		while((ch = GetCh()) > 0) {
			if(ch == '\r') {
				ch = GetCh();
				if(ch != '\n') {
					UngetCh( ch );
				}
				m_nLine++;
				return strLine;
			} else {
				strLine.push_back( ch );
			}
		}
		return strLine;
	}

	//! 終了したかどうか
	/*!
		@return 終了してたらtrue \n
				してなかったらfalseを返す
	*/
	bool eof( void ) {
		return m_fEof;
	}

	//! 1文字取得
	/*!
		@return	文字 \n
				失敗、終わってたら負数を返す
	*/
	int32_t GetCh( void ) {
		if(m_fUnget) {
			m_fUnget = false;
			m_nOffset++;
			return m_nChar;
		}
		uint8_t c;
		if(Cat_StreamRead( m_pStream, &c, 1 ) != 1) {
			m_fEof = true;
			return -1;
		}
		m_nOffset++;
		return (int32_t)c;
	}

	//! 1文字戻す
	/*!
		@param[in]	nChar	文字
	*/
	void UngetCh( int32_t nChar ) {
		m_nOffset--;
		m_nChar = nChar;
		m_fUnget = true;
	}

	//! 行番号を取得する
	uint32_t GetLineNo( void ) const {
		return m_nLine;
	}

	//! 位置を取得する
	uint32_t GetOffset( void ) const {
		return m_nOffset;
	}

private:
	Cat_Stream*	m_pStream;
	uint32_t	m_nLine;
	uint32_t	m_nOffset;
	bool		m_fUnget;
	int32_t		m_nChar;
	bool		m_fEof;
	bool		m_fClose;
};

//! コンストラクタ
/*!
	@param[in]	pStream	ストリーム
	@param[in]	fClose	終了時にストリームを閉じるかどうか
*/
icTextReader::icTextReader( Cat_Stream* pStream, bool fClose )
	: m_impl( new icTextReaderImpl( pStream, fClose ) )
{
}

//! 行番号を取得する
uint32_t
icTextReader::GetLineNo( void ) const
{
	return m_impl->GetLineNo();
}

//! 位置を取得する
uint32_t
icTextReader::GetOffset( void ) const
{
	return m_impl->GetOffset();
}

//! １行読み込む
/*!
	@return	改行コードを含まない文字列
*/
std::string
icTextReader::ReadLine( void )
{
	return m_impl->ReadLine();
}

//! 終了したかどうか
/*!
	@return 終了してたらtrue \n
			してなかったらfalseを返す
*/
bool
icTextReader::eof( void )
{
	return m_impl->eof();
}

} // namespace ic
