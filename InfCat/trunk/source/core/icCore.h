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

// boost
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>

// InfCat
#include "icTexture.h"
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
