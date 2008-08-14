//! @file	icAct.cp
// パレット

#include "icCore.h"

namespace ic {

//! コンストラクタ
icAct::icAct()
	: m_pPalette( 0 )
{
}

//! 作成する
/*!
	@param[in]	pStream	ストリーム
	@return	成功したらtrue \n
			失敗したらfalseを返す
*/
bool
icAct::Create( Cat_Stream* pStream )
{
	Release();

	bool rc = false;
	if(pStream) {
		uint32_t* pnPalette = (uint32_t*)CAT_MALLOC( sizeof(uint32_t) * 256 );
		if(pnPalette) {
			uint8_t* pbReadBuffer = (uint8_t*)CAT_MALLOC( 3 * 256 );
			if(pbReadBuffer) {
				if(Cat_StreamRead( pStream, pbReadBuffer, 3 * 256 ) == 3 * 256) {
					// 順序を逆に
					uint32_t j = 255;
					for(uint32_t i = 0; i < 256 * 3; i += 3) {
						pnPalette[j] =
							   (uint32_t)pbReadBuffer[i + 0]
							| ((uint32_t)pbReadBuffer[i + 1] << 8)
							| ((uint32_t)pbReadBuffer[i + 2] << 16)
							| 0xFF000000U;
						j--;
					}
					m_pPalette = Cat_PaletteCreate( FORMAT_PALETTE_8888, 256, pnPalette );
					if(m_pPalette) {
						rc = true;
					}
				}
				CAT_FREE( pbReadBuffer );
			}
			CAT_FREE( pnPalette );
		}
	}
	return rc;
}

//! 解放する
void
icAct::Release( void )
{
	if(m_pPalette) {
		Cat_PaletteRelease( m_pPalette );
		m_pPalette = 0;
	}
}

//! パレットを取得する
/*!
	@return	パレット
*/
Cat_Palette*
icAct::GetPalette( void )
{
	return m_pPalette;
}

} // namespace ic
