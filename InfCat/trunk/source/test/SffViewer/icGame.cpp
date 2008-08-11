#include "icCore.h"
#include "icGame.h"
#include "icGameSffViewer.h"

namespace ic {

//! ゲーム
icGame* icGame::m_pGame = 0;

//! メインループ
/*!
	ゲームのメインループの処理を行う
	@return ゲームループを継続する場合はtrue \n
			ゲームループを終了する場合はfalseを返す
*/
bool
icGame::MainLoop( void )
{
	bool rc = true;
	if(m_pGame) {
		rc = m_pGame->Framemove();
		m_pGame->Render();
	}
	return rc;
}

//! ゲームモード切り替え
/*!
	@param[in]	eGameMode	ゲームモード
	@param[in]	pvInitParam	初期化パラメータ
*/
void
icGame::ChangeGameMode( enumGameMode eGameMode, const void* pvInitParam )
{
	if(m_pGame) {
		m_pGame->Terminate();
		delete m_pGame;
		m_pGame = 0;
	}
	switch(eGameMode) {
		case eGameMode_SffViewer:
			m_pGame = new icGameSffViewer;
			break;
		default:
			m_pGame = new icGame;
			break;
	}
	if(m_pGame) {
		if(!m_pGame->Initialize( pvInitParam )) {
			/* 初期化失敗 */
			delete m_pGame;
			m_pGame = 0;
		}
	}
}

} // namespace ic
