//! @file	main.cpp
// キャラクタ Defファイル 読み込み - テスト用

#include "icCore.h"

using namespace ic;

//! 読み込むファイル名
#define FILENAME "test.def"

static const char* tblIndex[] = {
	"Info", "name",
	"Info", "displayname",
	"Info", "versiondate",
	"Info", "mugenversion",
	"Info", "author",
	"Info", "pal.defaults",

	"Files", "cmd",
	"Files", "cns",
	"Files", "st",
	"Files", "st1",
	"Files", "st2",
	"Files", "st3",
	"Files", "st4",
	"Files", "stcommon",
	"Files", "sprite",

	"Arcade", "intro.storyboard",

	0,0
};

int
main()
{
	Cat_SetupCallbacks();
	pspDebugScreenInit();

	Cat_Stream* pStream = Cat_StreamFileReadOpen( FILENAME );
	if(pStream == 0) {
		TRACE(( "%s not found", FILENAME ));
		HALT();
	}

	icDef def;
	if(!def.Load( pStream )) {
		TRACE(( "%s read error", FILENAME ));
		HALT();
	}

	for(int32_t i = 0; tblIndex[i]; i += 2) {
		TRACE(( "%s - %s - ", tblIndex[i], tblIndex[i + 1] ));
		int32_t nCount = def.Count( tblIndex[i], tblIndex[i + 1] );
		if(nCount) {
			for(int32_t j = 0; j < nCount; j++) {
				TRACE(( "%s ", def.GetValue( tblIndex[i], tblIndex[i + 1], j ).c_str() ));
			}
			TRACE(( "\n" ));
		} else {
			TRACE(( "not found\n" ));
		}
	}

	Cat_StreamClose( pStream );
	HALT();

	return 0;
}
