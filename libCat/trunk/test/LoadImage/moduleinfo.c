#include <pspmoduleinfo.h>
#include <pspthreadman.h>

PSP_MODULE_INFO( "LoadImage", PSP_MODULE_USER, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

PSP_HEAP_SIZE_MAX();
PSP_MAIN_THREAD_STACK_SIZE_KB(128);
