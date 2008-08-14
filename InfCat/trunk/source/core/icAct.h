//! @file	icAct.h
// パレット

#ifndef INCL_CLASS_icAct
#define INCL_CLASS_icAct

#include "icCore.h"

namespace ic {

//! パレット
class icAct {
public:
	//! コンストラクタ
	icAct();

	//! 作成する
	/*!
		@param[in]	pStream	ストリーム
		@return	成功したらtrue \n
				失敗したらfalseを返す
	*/
	bool Create( Cat_Stream* pStream );

	//! 解放する
	void Release( void );

	//! パレットを取得する
	/*!
		@return	パレット
	*/
	Cat_Palette* GetPalette( void );
private:
	Cat_Palette*	m_pPalette;		/*!< パレット	*/
};

} // namespace ic

#endif // INCL_CLASS_icAct
