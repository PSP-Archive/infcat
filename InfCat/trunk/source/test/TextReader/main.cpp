//! @file	main.cpp
// TextReader - テスト用

#include "icCore.h"

using namespace ic;

//! 読み込むファイル名
#define FILENAME "test.txt"

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

	icTextReader reader( pStream );

	while(!reader.eof()) {
		uint32_t nLineNo = reader.GetLineNo();
		uint32_t nOffset = reader.GetOffset();
		std::string strLine = reader.ReadLine();
		TRACE(( "%3d:%5d:%s\n", (int)nLineNo, (int)nOffset, strLine.c_str() ));
	}

	Cat_StreamClose( pStream );

	TRACE(( "[EOF]\n" ));
	HALT();

	return 0;
}
