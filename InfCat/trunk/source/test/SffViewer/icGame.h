//! @file	icGame.h
// ゲームモード管理

#ifndef INCL_CLASS_icGame
#define INCL_CLASS_icGame

namespace ic {

//! ゲームモード
enum enumGameMode {
	eGameMode_SffViewer
};

//! ゲームクラス
class icGame {
public:
	icGame() {}
	virtual ~icGame() {}

	//! 初期化
	/*!
		@param[in]	pvInitParam	初期化パラメータ
		@return ゲームループを継続する場合はtrue \n
				ゲームループを終了する場合はfalseを返す
	*/
	virtual bool Initialize( const void* pvInitParam ) { return true; }

	//! 更新
	virtual bool Framemove( void ) { return true; };

	//! 描画
	virtual void Render( void ) {};

	//! 終了処理
	virtual void Terminate( void ) {};

	//! メインループ
	/*!
		ゲームのメインループの処理を行う
		@return ゲームループを継続する場合はtrue \n
				ゲームループを終了する場合はfalseを返す
	*/
	static bool MainLoop( void );

	//! ゲームモード切り替え
	/*!
		@param[in]	eGameMode	ゲームモード
		@param[in]	pvInitParam	初期化パラメータ
	*/
	static void ChangeGameMode( enumGameMode eGameMode, const void* pvInitParam = 0 );
private:
	static icGame* m_pGame;		/*!< ゲーム */
};

} // namespace ic

#endif // INCL_CLASS_icGame
