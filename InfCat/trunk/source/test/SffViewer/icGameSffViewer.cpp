//! @file	icGameSffViewer.cpp
// Sff閲覧 - テスト用

#include "icCore.h"
#include "icGameSffViewer.h"

namespace ic {

//! 描画
static void
RenderSprite( float x, float y, float z, float w, float h, float tw, float th )
{
	struct vertex_format {
		short   u,v;
		short	x,y,z;
	} __attribute__((packed)) * vert = (struct vertex_format*)sceGuGetMemory( 32 );
	vert[0].u     = 0;
	vert[0].v     = 0;
	vert[0].x     = (short)x;
	vert[0].y     = (short)y;
	vert[0].z     = (short)z;
	vert[1].u     = (short)tw;
	vert[1].v     = (short)th;
	vert[1].x     = (short)(x + w);
	vert[1].y     = (short)(y + h);
	vert[1].z     = (short)z;
	sceGuDrawArray( GU_SPRITES, GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D, 2, 0, vert );
}

//! 実装
class icGameSffViewerImpl {
public:
	//! コンストラクタ
	icGameSffViewerImpl()
		: m_pTexturePool( new icTexturePool )
		, m_pTexture( 0 )
		, m_nIndex( 0 )
		, m_nActIndex( 0 )
		, m_pAct( new icAct )
	{
	}

	//! 初期化
	/*!
		@param[in]	pvInitParam	初期化パラメータ
		@return ゲームループを継続する場合はtrue \n
				ゲームループを終了する場合はfalseを返す
	*/
	bool Initialize( const void* pvInitParam ) {
		bool rc = false;
		Cat_Stream* pStream = Cat_StreamFileReadOpen( (const char*)pvInitParam );
		if(pStream) {
			if(m_pTexturePool && m_pTexturePool->Create( pStream )) {
				m_pTexture = m_pTexturePool->SearchFromIndex( m_nIndex );
				rc = true;
			} else {
				TRACE(( "%s load failed.", (const char*)pvInitParam ));
				HALT();
			}
			Cat_StreamClose( pStream );
		}
		else {
			TRACE(( "%s not found.", (const char*)pvInitParam ));
			HALT();
		}
		return rc;
	}

	//! 更新
	bool Framemove( void ) {
		if(m_pTexturePool) {
			uint32_t nPreIndex = m_nIndex;
			uint32_t nPreActIndex = m_nActIndex;
			if(Cat_InputGetPressed( 0 ) & CAT_INPUT_LEFT) {
				if(m_nIndex > 0) {
					m_nIndex--;
				} else {
					m_nIndex = m_pTexturePool->GetTextureCount() - 1;
				}
			}
			if(Cat_InputGetPressed( 0 ) & CAT_INPUT_RIGHT) {
				if(m_nIndex < (m_pTexturePool->GetTextureCount() - 1)) {
					m_nIndex++;
				} else {
					m_nIndex = 0;
				}
			}
			if(Cat_InputGetPressed( 0 ) & CAT_INPUT_UP) {
				if(m_nActIndex > 0) {
					m_nActIndex--;
				}
			}
			if(Cat_InputGetPressed( 0 ) & CAT_INPUT_DOWN) {
				if(m_nActIndex < 12) {
					m_nActIndex++;
				}
			}
			if(nPreActIndex != m_nActIndex) {
				char pszFilename[64];
				sprintf( pszFilename, "test%02d.act", (int)m_nActIndex );
				Cat_Stream* pStream = Cat_StreamFileReadOpen( pszFilename );
				if(pStream) {
					if(m_pAct->Create( pStream )) {
						m_pTexturePool->SetAct( m_pAct->GetPalette() );
					}
					Cat_StreamClose( pStream );
				} else {
					m_nActIndex = nPreActIndex;
				}
			}
			if(nPreIndex != m_nIndex) {
				m_pTexture = m_pTexturePool->SearchFromIndex( m_nIndex );
			}
		}
		return true;
	}

	//! 描画
	void Render( void ) {
		if(m_pTexture) {
			m_pTexture->SetTexture();
			const float x = 240.0f;
			const float y = 272.0f / 2.0f;
			const float z = 0.0f;
			const float w = (float)m_pTexture->GetWidth();
			const float h = (float)m_pTexture->GetHeight();
			const float tw = (float)m_pTexture->GetRealWidth();
			const float th = (float)m_pTexture->GetRealHeight();
			const float fOffsetX = (float)m_pTexture->GetDrawOffsetX();
			const float fOffsetY = (float)m_pTexture->GetDrawOffsetY();
			RenderSprite( x - fOffsetX, y - fOffsetY, z, w, h, tw, th );
		}
	}

	//! 終了処理
	void Terminate( void ) {
	}
private:
	boost::shared_ptr<icTexturePool>	m_pTexturePool;	/*!< テクスチャプール		*/
	icTexture*							m_pTexture;		/*!< 描画するテクスチャ		*/
	uint32_t							m_nIndex;		/*!< インデックス			*/
	uint32_t							m_nActIndex;	/*!< 適応しているパレット	*/
	boost::shared_ptr<icAct>			m_pAct;
};

//! コンストラクタ
icGameSffViewer::icGameSffViewer()
	: m_impl( new icGameSffViewerImpl )
{
}

//! 初期化
/*!
	@param[in]	pvInitParam	初期化パラメータ
	@return ゲームループを継続する場合はtrue \n
			ゲームループを終了する場合はfalseを返す
*/
bool
icGameSffViewer::Initialize( const void* pvInitParam )
{
	return m_impl->Initialize( pvInitParam );
}

//! 更新
bool
icGameSffViewer::Framemove( void )
{
	return m_impl->Framemove();
}

//! 描画
void
icGameSffViewer::Render( void )
{
	m_impl->Render();
}

//! 終了処理
void
icGameSffViewer::Terminate( void )
{
	m_impl->Terminate();
}

} // namespace ic
