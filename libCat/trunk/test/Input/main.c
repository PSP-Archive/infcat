// Cat_Input test code

#include "Cat_PspCallback.h"
#include "Cat_Input.h"
#include <stdlib.h>
#include <string.h>

#include <pspdebug.h>
#include <pspkernel.h>
#include <pspgu.h>
#include <pspdisplay.h>

#define LOCATE(x,y) pspDebugScreenSetXY( (x), (y) )
#define TRACE(x) pspDebugScreenPrintf x
#define HALT() sceKernelSleepThreadCB()


int
main()
{
	uint32_t nPlayerIndex;

	Cat_SetupCallbacks();
	pspDebugScreenInit();

	TRACE(( "Cat_Input test code\n" ));

	Cat_InputInit();
	// Cat_InputSetLaytence( 3 );	/* レイテンシの設定 */
	for(;;) {
		Cat_InputUpdate();			/* 入力の更新 */
		for(nPlayerIndex = 0; nPlayerIndex < 8; nPlayerIndex++) {
			int nCurrent  = Cat_InputGetCurrent( nPlayerIndex );	// 現在
			int nPrevious = Cat_InputGetPrevious( nPlayerIndex );	// 1個前
			int nTrigger  = Cat_InputGetTrigger( nPlayerIndex );	// 変化のあったところ
			int nPressed  = Cat_InputGetPressed( nPlayerIndex );	// 押された
			int nRelease  = Cat_InputGetRelease( nPlayerIndex );	// 離された
			LOCATE( 0, nPlayerIndex + 1 );
			TRACE(( "Cur:%08X Prev:%08X Trig:%08X Pres:%08X Rel:%08X", nCurrent, nPrevious, nTrigger, nPressed, nRelease ));
		}
		sceDisplayWaitVblankStartCB();
	}
	Cat_InputTerm();

	HALT();
	return 0;
}
