//! @file	icGameSffViewer.h
// Sff閲覧 - テスト用

#ifndef INCL_CLASS_icGameSffViewer
#define INCL_CLASS_icGameSffViewer

#include "icGame.h"

namespace ic {

//! Sffビューア
class icGameSffViewer : public icGame {
public:
	//! コンストラクタ
	icGameSffViewer();

	//! 初期化
	/*!
		@param[in]	pvInitParam	初期化パラメータ
		@return ゲームループを継続する場合はtrue \n
				ゲームループを終了する場合はfalseを返す
	*/
	virtual bool Initialize( const void* pvInitParam );

	//! 更新
	virtual bool Framemove( void );

	//! 描画
	virtual void Render( void );

	//! 終了処理
	virtual void Terminate( void );
private:
	boost::shared_ptr<class icGameSffViewerImpl>	m_impl;	/*!< 実装	*/
};

} // namespace ic

#endif // INCL_CLASS_icGameSffViewer
