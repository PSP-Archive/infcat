//! @file	main.cpp
// Sff閲覧 - テスト用

#include "icCore.h"
#include "icGame.h"

using namespace ic;

//! 読み込むファイル名
#define FILENAME "test.sff"

int
main()
{
	Cat_SetupCallbacks();
	pspDebugScreenInit();

	icTextureCreatorSff sff;	// SFFテクスチャ作成の登録

	Cat_RenderInit( CAT_RENDER_DEFAULT );
	Cat_InputInit();
	icGame::ChangeGameMode( eGameMode_SffViewer, FILENAME );	// SffViewerにしとく
	bool fContinue = true;
	while(fContinue) {
		Cat_InputUpdate();					// 入力の更新
		Cat_RenderBegin(); {				// 描画開始
			fContinue = icGame::MainLoop();
		} Cat_RenderEnd();					// 描画終了
		Cat_RenderScreenUpdate();			// 画面更新
	}
	Cat_InputTerm();
	Cat_RenderTerm();

	return 0;
}
