//! @file	icCore.h

#ifndef INCL_icCore_h
#define INCL_icCore_h

// stl
#include <vector>
#include <list>
#include <map>

// libCat
#include "Cat_PspCallback.h"
#include "Cat_ImageLoader.h"
#include "Cat_StreamFile.h"
#include "Cat_Render.h"
#include "Cat_Input.h"

#ifndef CAT_MALLOC
//! メモリ確保マクロ
#define CAT_MALLOC(x) memalign( 32, (x) )
#endif // CAT_MALLOC

#ifndef CAT_FREE
//! メモリ解放マクロ
#define CAT_FREE(x) free( x )
#endif // CAT_FREE

// boost
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>

// InfCat
#include "icAct.h"
#include "icTexture.h"
#include "icTexturePool.h"
#include "icSffLoader.h"

// for DEBUG
#include <pspdebug.h>
#include <pspkernel.h>
#include <pspgu.h>
#define TRACE(x) pspDebugScreenPrintf x
#define HALT() sceKernelSleepThreadCB()

// etc.
#include <malloc.h>	// for memalign
#include <string.h>

#endif // INCL_icCore_h
