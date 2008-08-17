//! @file	icDef.cpp
// キャラクタ定義ファイル

#include "icCore.h"

namespace ic {

//! トークン
class icToken {
public:
	bool Check( int32_t nType ) const {
		return m_nType == nType;
	}
	void Clear( void ) {
		m_strString = "";
	}

	std::string GetString( void ) const {
		return m_strString;
	}
	uint32_t GetLineNo( void ) const {
		return m_nLineNo;
	}
	uint32_t GetOffset( void ) const {
		return m_nOffset;
	}

	void SetType( int32_t nType ) {
		m_nType = nType;
	}
	void SetLineNo( uint32_t nLineNo ) {
		m_nLineNo = nLineNo;
	}
	void SetOffset( uint32_t nOffset ) {
		m_nOffset = nOffset;
	}

	std::string m_strString;
private:
	int32_t		m_nType;
	uint32_t	m_nLineNo;
	uint32_t	m_nOffset;
};

//! 字句解析のベースクラス
class icScannerBase {
public:
	enum {
		eTokenTypeEOT,			/*!< 終端	*/
		eTokenTypeString,		/*!< 文字列	*/
	};

	//! コンストラクタ
	/*!
		@param[in]	strString	文字列
		@param[in]	nLineNo		行番号
		@param[in]	nOffset		オフセット
	*/
	icScannerBase( const std::string& strString, uint32_t nLineNo, uint32_t nOffset )
		: m_strString( strString )
		, m_nIndex( 0 )
		, m_fUnget( false )
		, m_nUngetCh( 0 )
		, m_fUngetToken( false )
		, m_nLineNo( nLineNo )
		, m_nOffset( nOffset )
	{
		m_TokenEOT.SetType( eTokenTypeEOT );
		m_TokenEOT.SetLineNo( m_nLineNo );
		m_Token.SetLineNo( m_nLineNo );
	}

	//! デストラクタ
	virtual ~icScannerBase() {
	}

	//! トークンを取得する
	/*!
		@return トークン
	*/
	icToken& GetToken( void ) {
		if(m_fUngetToken) {
			m_fUngetToken = false;
			return m_UngetToken;
		}
		m_Token.SetOffset( m_nOffset + m_nIndex );
		m_TokenEOT.SetOffset( m_nOffset + m_nIndex );
		return GetTokenInner();
	}

	//! トークンを戻す
	/*!
		@param[in]	token	トークン
	*/
	virtual void UngetToken( icToken& token ) {
		m_fUngetToken = true;
		m_UngetToken = token;
	}
protected:
	//! トークンを取得する
	/*!
		@return トークン
	*/
	virtual icToken& GetTokenInner( void ) = 0;

	//! 空白文字かどうか
	/*!
		@param[in]	ch	文字
		@return	空白文字ならtrue \n
				それ以外ならfalseを返す
	*/
	virtual bool IsSpace( int32_t ch ) {
		return (ch == ' ') || (ch == '\t');
	}

	//! 区切文字かどうか
	/*!
		@param[in]	ch	文字
		@return	区切文字ならtrue \n
				それ以外ならfalseを返す
	*/
	virtual bool IsSeparator( int32_t ch ) {
		return (ch == ' ')
			|| (ch == '\t')
			|| (ch == '[')
			|| (ch == ']')
			|| (ch == '=')
			|| (ch == ',')
			|| (ch == '"')
			|| (ch < 0);
	}

	//! 空白文字をスキップする
	void SkipSpace( void ) {
		int32_t ch;
		while(IsSpace( (ch = GetCh()) )) {
		}
		UngetCh( ch );
	}

	//! 1文字戻す
	/*!
		@param[in]	ch	文字
	*/
	void UngetCh( int32_t ch ) {
		m_fUnget = true;
		m_nUngetCh = ch;
	}

	//! 1文字取得する
	/*!
		@return	文字
				終わったら負数を返す
	*/
	int32_t GetCh( void ) {
		if(m_fUnget) {
			m_fUnget = false;
			return m_nUngetCh;
		}
		if(m_nIndex >= m_strString.size()) {
			return -1;
		}
		return m_strString[m_nIndex++];
	}
private:
	std::string	m_strString;		/*!< 文字列							*/
	uint32_t	m_nIndex;			/*!< 文字位置						*/
	bool		m_fUnget;			/*!< 戻された文字があるかどうか		*/
	int32_t 	m_nUngetCh;			/*!< 戻された文字					*/
	bool		m_fUngetToken;		/*!< 戻されたトークンがあるかどうか	*/
	icToken		m_UngetToken;		/*!< 戻されたトークン				*/
	uint32_t	m_nLineNo;			/*!< 行番号							*/
	uint32_t	m_nOffset;			/*!< オフセット						*/
protected:
	icToken m_Token;				/*!< トークン						*/
	icToken m_TokenEOT;				/*!< 終端のトークン					*/
};

//! キャラクタ定義ファイル用字句解析
class icScanner : public icScannerBase {
public:
	//! コンストラクタ
	/*!
		@param[in]	strString	文字列
	*/
	icScanner( const std::string& strString, uint32_t nLineNo, uint32_t nOffset )
		: icScannerBase( strString, nLineNo, nOffset )
	{
	}

	//! トークンを取得する
	/*!
		@return トークン
	*/
	virtual icToken& GetTokenInner( void ) {
		m_Token.Clear();
		SkipSpace();
		int32_t ch = GetCh();
		switch(ch) {
			case ';':
				return m_TokenEOT;
			case '"':
				m_Token.SetType( icScannerBase::eTokenTypeString );
				while(1) {
					ch = GetCh();
					if(ch < 0) {
						return m_TokenEOT;
					}
					if(ch == '"') {
						return m_Token;
					}
					m_Token.m_strString.push_back( ch );
				}
			case '=':
			case ',':
			case '[':
			case ']':
				m_Token.SetType( ch );
				m_Token.m_strString.push_back( ch );
				return m_Token;
			default:
				if(ch < 0) {
					return m_TokenEOT;
				}
				m_Token.SetType( icScannerBase::eTokenTypeString );
				m_Token.m_strString.push_back( ch );
				while(!IsSeparator( (ch = GetCh()) )) {
					m_Token.m_strString.push_back( ch );
				}
				UngetCh( ch );
				return m_Token;
		}
	}
};

//! キャラクタ定義ファイルを読み込む
/*!
	@param[in]	pStream	ストリーム
	@return 成功した場合true \n
			失敗したらfalseを返す
*/
bool
icDef::Load( Cat_Stream* pStream )
{
	// LINE := '\n'
	//      | '[' SECTION
	//      | string KEY_VALUE
	// SECTION   := string ']' '\n'
	// KEY_VALUE := '=' string [ ',' string ] '\n'

	std::string strSection;
	std::string strKey;
	std::string strValue;

	m_SectionValue.Clear();

	icTextReader reader( pStream );
	while(!reader.eof()) {
		const std::string strLine = reader.ReadLine();
		icScanner scanner( strLine, reader.GetLineNo(), reader.GetOffset() );

		icToken token = scanner.GetToken();
		if(token.Check( '[' )) {
			// SECTION := string ']' '\n'
			token = scanner.GetToken();
			if(!token.Check( icScannerBase::eTokenTypeString )) {
				// 文字列ではない
				return false;
			}
			strSection = token.GetString();
			if(!scanner.GetToken().Check( ']' )) {
				// ']'がない
				return false;
			}
			if(!scanner.GetToken().Check( icScannerBase::eTokenTypeEOT )) {
				return false;
			}
		} else if(token.Check( icScannerBase::eTokenTypeString )) {
			// KEY_VALUE := '=' string [ ',' string ] '\n'
			strKey = token.GetString();
			m_SectionValue.Clear( strSection, strKey );
			if(!scanner.GetToken().Check( '=' )) {
				// '='がない
				return false;
			}
			token = scanner.GetToken();
			if(!token.Check( icScanner::eTokenTypeString )) {
				// 文字列ではない
				return false;
			}
			strValue = token.GetString();
			m_SectionValue.Register( strSection, strKey, strValue );

			token = scanner.GetToken();
			while(token.Check( ',' )) {
				token = scanner.GetToken();
				if(!token.Check( icScannerBase::eTokenTypeString )) {
					// stringがない
					return false;
				}
				strValue = token.GetString();
				m_SectionValue.Register( strSection, strKey, strValue );
				token = scanner.GetToken();
			}
			if(!token.Check( icScannerBase::eTokenTypeEOT )) {
				return false;
			}
		} else if(!token.Check( icScannerBase::eTokenTypeEOT )) {
			return false;
		}
	}
	return true;
}

//! 値の項目が幾つあるか
/*!
	@param[in]	strSection	セクション
	@param[in]	strKey		キー
	@return 値の項目数
*/
int32_t
icDef::Count( const std::string& strSection, const std::string& strKey )
{
	return m_SectionValue.Count( strSection, strKey );
}

//! 値を取得する
/*!
	@param[in]	strSection	セクション
	@param[in]	strKey		キー
	@param[in]	nIndex		値のインデックス
	@return 値
*/
std::string
icDef::GetValue( const std::string& strSection, const std::string& strKey, int32_t nIndex )
{
	return m_SectionValue.GetValue( strSection, strKey, nIndex );
}

} // namespace ic
