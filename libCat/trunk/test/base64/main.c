// Cat_Base64 test code

#include "Cat_PspCallback.h"
#include "Cat_Base64.h"
#include <stdlib.h>
#include <string.h>

#include <pspdebug.h>
#include <pspkernel.h>

#define TRACE(x) pspDebugScreenPrintf x
#define HALT() sceKernelSleepThreadCB()

int
main()
{
	char pszEncode[0x100];
	char pszDecode[0x100];
	size_t nSize;

	Cat_SetupCallbacks();
	pspDebugScreenInit();

	TRACE(( "Cat_Base64 test code\n" ));

	// 文字列        : "ABCDEFG"
	// 16進数        : 41, 42, 43, 44, 45, 46, 47
	// 2進数         : 0100 0001, 0100 0010, 0100 0011, 0100 0100, 0100 0101, 0100 0110, 0100 0111
	// 6bitずつに分割  010000 010100 001001 000011 010001 000100 010101 000110 010001 11
	// 6bitにする      010000 010100 001001 000011 010001 000100 010101 000110 010001 110000
	// 4文字ずつ変換   "QUJD","REVG","Rw"
	// 4文字にする     "QUJD","REVG","Rw=="
	// Base64文字列  : "QUJDREVGRw=="

	nSize = Cat_Base64Encode( "ABCDEFG", 7, 0 );
	if(nSize != (12 + 1)) {
		TRACE(( "Error:Cat_Base64Encode Size:%d", nSize ));
		HALT();
	}
	nSize = Cat_Base64Encode( "ABCDEFG", 7, pszEncode );
	if(memcmp( pszEncode, "QUJDREVGRw==", 12 + 1 ) != 0) {
		TRACE(( "Error:Cat_Base64Encode Size:%d %s not match", nSize, pszEncode ));
		HALT();
	}

	nSize = Cat_Base64Decode( pszEncode, 0, 0 );
	if(nSize != 7) {
		TRACE(( "Error:Cat_Base64Decode Size:%d", nSize ));
		HALT();
	}

	memset( pszDecode, 0x7F, 0x100 );
	nSize = Cat_Base64Decode( pszEncode, pszDecode, 7 );
	if(pszDecode[7] != 0x7F) {
		TRACE(( "Error:Cat_Base64Decode Size:%d string over", nSize ));
		HALT();
	}
	if(memcmp( pszDecode, "ABCDEFG", 7 ) != 0) {
		pszDecode[0x100 - 1] = 0;
		TRACE(( "Error:Cat_Base64Decode Size:%d %s not match", nSize, pszDecode ));
		HALT();
	}

	TRACE(( "ok" ));
	HALT();
	return 0;
}
